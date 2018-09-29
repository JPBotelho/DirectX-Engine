/*
 * Handles writing text to a .txt file (used for logging)
 */
#pragma once
class LogManager
{
	public:
		LogManager();
		~LogManager();
		void Open();
		void Close();
		void Append(char *content);
};

