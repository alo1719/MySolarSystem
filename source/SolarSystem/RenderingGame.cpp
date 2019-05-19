#include "pch.h"
#include "RenderingGame.h"	

using namespace std;
using namespace DirectX;
using namespace Library;

namespace Rendering
{
	const XMVECTORF32 RenderingGame::BackgroundColor = Colors::Black;
	
	RenderingGame::RenderingGame(std::function<void*()> getWindowCallback, std::function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback), mRenderStateHelper(*this)
	{
	}

	// ���г�ʼ��
	void RenderingGame::Initialize()
	{
		// ����ͼ
		if (directionMode == 1)
		{
			RenderingGame::sCameraRotation = -50.0f; // �ӽ���ʱ����ת����, Ĭ��Ϊ-70.0f, Ĭ�ϳ���-Z����
			RenderingGame::sCameraPosition = XMFLOAT3(-1200.0f, 100.0f, 1200.0f); // ��Զ, ����, ���
		}

		// ����ͼ
		if (directionMode == 2)
		{
			RenderingGame::sCameraRotation = -90.0f; // �ӽ���ʱ����ת����
			RenderingGame::sCameraPosition = XMFLOAT3(0.0f, 2400.0f, 000.0f); // ��Զ, ����, ���
		}

		RasterizerStates::Initialize(mDirect3DDevice.Get());
		SamplerStates::Initialize(mDirect3DDevice.Get());

		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mMouse = make_shared<MouseComponent>(*this);
		mComponents.push_back(mMouse);
		mServices.AddService(MouseComponent::TypeIdClass(), mMouse.get());

		mCamera = make_shared<FirstPersonCamera>(*this);
		mComponents.push_back(mCamera);
		mServices.AddService(Camera::TypeIdClass(), mCamera.get());

		mSun = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Sun);
		const Library::PointLight& pointLight = mSun->GetLight();
		mComponents.push_back(mSun);

		mMercury = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Mercury);
		mMercury->SetLight(pointLight);
		mComponents.push_back(mMercury);

		mVenus = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Venus);
		mVenus->SetLight(pointLight);
		mComponents.push_back(mVenus);

		mEarth = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Earth);
		mEarth->SetLight(pointLight);
		mComponents.push_back(mEarth);

		mMoon = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Moon);
		mMoon->SetLight(pointLight);
		mMoon->SetParentObject(*mEarth);
		mComponents.push_back(mMoon);

		mMars = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Mars);
		mMars->SetLight(pointLight);
		mComponents.push_back(mMars);

		mJupiter = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Jupiter);
		mJupiter->SetLight(pointLight);
		mComponents.push_back(mJupiter);

		mSaturn = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Saturn);
		mSaturn->SetLight(pointLight);
		mComponents.push_back(mSaturn);

		mUranus = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Uranus);
		mUranus->SetLight(pointLight);
		mComponents.push_back(mUranus);

		mNeptune = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Neptune);
		mNeptune->SetLight(pointLight);
		mComponents.push_back(mNeptune);

		mPluto = make_shared<AstronomicalObject>(*this, mCamera, Rendering::AstronomicalObjectName::Pluto);
		mPluto->SetLight(pointLight);
		mComponents.push_back(mPluto);

		Game::Initialize();

		mFpsComponent = make_shared<FpsComponent>(*this);
		mFpsComponent->Initialize();

		// ����ͼ
		if (directionMode == 1)
		{
			XMMATRIX matrix = XMMatrixRotationY(XMConvertToRadians(sCameraRotation));
			mCamera->ApplyRotation(matrix);
		}
		// ����ͼ
		if (directionMode == 2)
		{
			XMMATRIX matrix = XMMatrixRotationX(XMConvertToRadians(sCameraRotation));
			mCamera->ApplyRotation(matrix);
		}
		mCamera->SetPosition(sCameraPosition.x, sCameraPosition.y, sCameraPosition.z);
	}

	void RenderingGame::Update(const GameTime &gameTime)
	{
		mFpsComponent->Update(gameTime);

		// �ж��Ƿ���Esc��
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape))
		{
			Exit();
		}

		Game::Update(gameTime);
	}

	void RenderingGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		Game::Draw(gameTime);

		mRenderStateHelper.SaveAll();
		mFpsComponent->Draw(gameTime);
		mRenderStateHelper.RestoreAll();

		HRESULT hr = mSwapChain->Present(1, 0);

		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
		}
		else
		{
			ThrowIfFailed(hr, "IDXGISwapChain::Present() failed.");
		}
	}

	void RenderingGame::Shutdown()
	{
		SamplerStates::Shutdown();
		RasterizerStates::Shutdown();
	}

	void RenderingGame::Exit()
	{
		PostQuitMessage(0);
	}
}