#include "forge/core/process_runner.hpp"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace forge {

ProcessResult ProcessRunner::run(const std::string& command, const std::string& workingDir) {
    ProcessResult result;
    result.exitCode = -1;

#ifdef _WIN32
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags |= STARTF_USESTDHANDLES;
    si.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    si.hStdError  = GetStdHandle(STD_ERROR_HANDLE);
    si.hStdInput  = GetStdHandle(STD_INPUT_HANDLE);

    ZeroMemory(&pi, sizeof(pi));

    std::string cmdCopy = command;
    const char* workDirPtr = workingDir.empty() ? NULL : workingDir.c_str();

    if (CreateProcessA(
            NULL,
            &cmdCopy[0],
            NULL,
            NULL,
            TRUE,
            0,
            NULL,
            workDirPtr,
            &si,
            &pi)) {

        WaitForSingleObject(pi.hProcess, INFINITE);

        DWORD exitCode = 0;
        if (GetExitCodeProcess(pi.hProcess, &exitCode)) {
            result.exitCode = static_cast<int>(exitCode);
        }

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
#else
    std::string fullCmd = command;
    if (!workingDir.empty()) {
        fullCmd = "cd " + workingDir + " && " + command;
    }
    int status = system(fullCmd.c_str());
    result.exitCode = WEXITSTATUS(status);
#endif

    return result;
}

}