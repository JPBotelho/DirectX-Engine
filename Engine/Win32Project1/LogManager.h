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

