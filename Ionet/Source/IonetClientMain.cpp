#include "Ionet/IonetClient.hpp"

int main() {
	ionet::IonetClient client("35.193.191.169", 60000);
	// ionet::IonetClient client("127.0.0.1", 60000);

	auto start = std::chrono::system_clock::now();

	bool key[5] = { false, false, false, false, false };
	bool old_key[5] = { false, false, false, false, false };

	while (client.IsConnected())
	{
		//auto now = std::chrono::system_clock::now();
		//auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - start);
		//if (diff > std::chrono::seconds(5)) {
		//	start = now;
		//	std::cout << "Sending ping..." << std::endl;
		//	client.Ping();

		//}
		if (GetForegroundWindow() == GetConsoleWindow())
		{

			key[0] = GetAsyncKeyState('1') & 0x8000;
			key[1] = GetAsyncKeyState('2') & 0x8000;
			key[2] = GetAsyncKeyState('3') & 0x8000;
			key[3] = GetAsyncKeyState('4') & 0x8000;
			key[4] = GetAsyncKeyState('5') & 0x8000;
		}

		if (key[0] && !old_key[0]) client.Ping();
		if (key[1] && !old_key[1]) client.JoinRoom(0);
		if (key[2] && !old_key[2]) client.JoinRoom(1);
		if (key[3] && !old_key[3]) client.ListRooms();
		if (key[4] && !old_key[4]) client.PrintRooms();
		for (int i = 0; i < 5; i++) old_key[i] = key[i];

		client.Update();

	}
	system("pause");
	return 0;
}