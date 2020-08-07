#include "Paths.h"

static HMODULE ourModule;
static std::string moduleFolder;
static std::string moduleName;
static std::string moduleNameWithoutExt;

const std::string Paths::GetRunningExecutableFolder() {

	char fileName[MAX_PATH];
	GetModuleFileNameA(NULL, fileName, MAX_PATH);

	std::string currentPath = fileName;
	return currentPath.substr(0, currentPath.find_last_of("\\"));
}

const std::string Paths::GetRunningExecutableName() {

	char fileName[MAX_PATH];
	GetModuleFileNameA(NULL, fileName, MAX_PATH);

	std::string fullPath = fileName;

	size_t lastIndex = fullPath.find_last_of("\\") + 1;
	return fullPath.substr(lastIndex, fullPath.length() - lastIndex);
}

const std::string Paths::GetRunningExecutableNameWithoutExtension() {
	const std::string fileNameWithExtension = GetRunningExecutableName();

	size_t lastIndex = fileNameWithExtension.find_last_of(".");
	if (lastIndex == -1) {
		return fileNameWithExtension;
	}

	return fileNameWithExtension.substr(0, lastIndex);
}

const std::string Paths::GetModuleFolder(const HMODULE module) {

	char fileName[MAX_PATH];
	GetModuleFileNameA(module, fileName, MAX_PATH);

	std::string currentPath = fileName;
	return currentPath.substr(0, currentPath.find_last_of("\\"));
}

const std::string Paths::GetModuleName(const HMODULE module) {

	char fileName[MAX_PATH];
	GetModuleFileNameA(module, fileName, MAX_PATH);

	std::string fullPath = fileName;

	size_t lastIndex = fullPath.find_last_of("\\") + 1;
	return fullPath.substr(lastIndex, fullPath.length() - lastIndex);
}

const std::string Paths::GetModuleNameWithoutExtension(const HMODULE module) {

	const std::string fileNameWithExtension = GetModuleName(module);

	size_t lastIndex = fileNameWithExtension.find_last_of(".");
	if (lastIndex == -1) {
		return fileNameWithExtension;
	}

	return fileNameWithExtension.substr(0, lastIndex);
}


void Paths::SetOurModuleHandle(const HMODULE module) {

	ourModule = module;
}

const HMODULE Paths::GetOurModuleHandle() {

	return ourModule;
}

void Paths::SetModuleInfo(std::string folder, std::string name) {
	moduleFolder = folder;
	moduleName = name;
	size_t lastIndex = moduleName.find_last_of(".");
	if (lastIndex == -1) {
		moduleNameWithoutExt = moduleName;
	}
	else {
		moduleNameWithoutExt = moduleName.substr(0, lastIndex);
	}
}

const std::string Paths::GetModuleFolder() {
	return moduleFolder;
}
const std::string Paths::GetModuleName() {
	return moduleName;
}
const std::string Paths::GetModuleNameWithoutExtension() {
	return moduleNameWithoutExt;
}
