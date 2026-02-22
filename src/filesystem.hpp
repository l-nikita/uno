#pragma once

#include <filesystem>
#include <string>

static std::string GetAssetsPath()
{
	return (std::filesystem::current_path().parent_path().parent_path() / "assets").string();
}