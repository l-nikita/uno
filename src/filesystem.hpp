#pragma once

#include <filesystem>
#include <string>

namespace fs 
{
	inline std::filesystem::path GetRootPath()
	{
		return std::filesystem::current_path().parent_path();
	}

	inline std::filesystem::path GetAssetsPath()
	{
		return (GetRootPath() / "assets");
	}
}