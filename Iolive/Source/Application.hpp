#pragma once

#define IOLIVE_GITHUB "https://github.com/nathanianah/Iolive"
#define IOLIVE_MAJOR_VERSION_STR "21"
#define IOLIVE_MINOR_VERSION_STR "06"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 700
#define WINDOW_TITLE "Iolive"

#include "MainGui.hpp"
#include "Window.hpp"
#include "Ioface/Ioface.hpp"
#include "Live2D/Model2D.hpp"
#include "Utility/JsonManager.hpp"
#include <thread>
#include <mutex>
#include <ionet/IonetClient.hpp>
#include "NetworkingClient.h"

namespace Iolive {
	constexpr double kCurrentJsonVersion = 0.1;
	constexpr wchar_t* kSettingsFileName = L"Iolive.settings.json";

	class UserModel
	{
	public:
		UserModel() = default;

		Model2D* GetModel2D() { return m_Model2D; }

		void SetModel(Model2D* newModel)
		{
			if (newModel)
				if (newModel->IsInitialized())
				{
					DeleteModel();
					m_Model2D = newModel;
				}
		}

		void DeleteModel()
		{
			if (m_Model2D) delete m_Model2D;
			m_Model2D = nullptr;
		}

		bool IsModelInitialized()
		{
			if (m_Model2D)
				if (m_Model2D->IsInitialized())
					return true;
			return false;
		}

	private:
		Model2D* m_Model2D = nullptr;
	};

	class Application : public NetworkingClient
	{
	public:
		inline static std::mutex s_MtxAppInstance;
		static Application* Get()
		{
			std::lock_guard<std::mutex> lock(s_MtxAppInstance);
			
			static Application* staticApplication = nullptr;
			if (staticApplication == nullptr)
				staticApplication = new Application();

			return staticApplication;
		}

		static void Release()
		{
			delete Application::Get();
		}

		void Run();

	private:
		Application();
		~Application();

		void OnUpdate();
		void OnRender();
		void OnNetworkUpdate();
		friend class MainGui;

		/* 
		* TODO: Capturing new frame until flags_StopCapture is true
		*/
		void FaceCaptureLoop();
		
		bool OpenCamera();
		void CloseCamera();

		void SetModel(Model2D* model);
		void CreateNewHotkeys(const wchar_t* outFilePath);
		void LoadHotkeys();
		void OnHotkeysSaved(int index, ModelMotion* motion);

		void LoadModelParams();

		void OnNetworkRequest(std::string address, uint16_t port, ionet::ClientMode client_mode);

		void DoOptimizeParameters();
		void BindDefaultParametersWithFace();
		void BindDefaultParametersWithGui();
		void BindDefaultParametersWithNetwork();

		/*
		* Window callback as static method
		*/
		static void OnFrameResizedCallback(int width, int height);
		static void OnScrollCallback(double xoffset, double yoffset);
		static void OnCursorPosCallback(bool pressed, double xpos, double ypos);

		// From networking client.
		bool IsConnected();
		void RequestRooms();
		void JoinRoom(ionet::RoomId room_id);
		void LeaveRoom();
		std::vector<ionet::RoomId> GetRooms();


	private:
		// Skeleton instance as member
		Window* m_Window;

		// Ioface
		Ioface m_Ioface;
		
		// UserModel for handling Model2D
		UserModel m_UserModel;

		// face capturing thread
		std::thread m_FaceCaptureThread;
		
		JsonManager m_JsonManager; //settings for model
		JsonManager m_AppJsonManager; //settings for window

		// Optimized parameter for Live2D model
		DefaultParameter::ParametersValue OptimizedParameter = {};

		// a flags
		bool flags_StopCapture;

		std::unique_ptr<ionet::IonetClient> m_client;
	};
} // namespace Iolive