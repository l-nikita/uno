#pragma once

#include <filesystem>
#include <string>

namespace fs 
{
	static std::filesystem::path GetRootPath()
	{
		return (std::filesystem::current_path().parent_path()).string();
	}

	static std::filesystem::path GetAssetsPath()
	{
		return (GetRootPath() / "assets");
	}
}