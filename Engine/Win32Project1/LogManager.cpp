#include "stdafx.h"
#include "LogManager.h"
#include <iostream>
#include <fstream>


LogManager::LogManager()
{
}


LogManager::~LogManager()
{
}

std::fstream streamer;

void LogManager::Open()
{
	remove("log.txt");
}

void LogManager::Append(char *content)
{
	if (!streamer.is_open())
	{
		streamer.open("log.txt", std::fstream::in | std::fstream::out | std::fstream::app);
	}
	streamer << content;
	streamer.sync();
	streamer.flush();
	streamer.close();
}
