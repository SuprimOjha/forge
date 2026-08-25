#include "forge/commands/completion.hpp"

#include <iostream>
#include <string>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

namespace forge {

namespace {

void enableConsoleEncoding() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
}

void printCompletionHelp() {
    std::cout
        << "\n"
        << "Forge Shell Completion Generator\n\n"

        << "Usage:\n"
        << "  forge completion <shell>\n\n"

        << "Supported Shells:\n"
        << "  bash, zsh, fish, powershell\n\n"

        << "Examples:\n"
        << "  forge completion powershell | Out-String | Invoke-Expression\n"
        << "  forge completion bash > ~/.forge-completion.bash\n";
}

void generatePowerShell() {
    std::cout << R"(
Register-ArgumentCompleter -Native -CommandName forge -ScriptBlock {
    param($wordToComplete, $commandAst, $cursorPosition)
    $commands = @('init','build','run','clean','test','doctor','info','stats','fmt','lint','scan','bench','bundle','deploy','watch','archive','license','diff','request','env','parallel','completion')
    $commands | Where-Object { $_ -like "$wordToComplete*" } | ForEach-Object {
        [System.Management.Automation.CompletionResult]::new($_, $_, 'ParameterValue', $_)
    }
}
)";
}

void generateBash() {
    std::cout << R"(
_forge_completions() {
    local cur="${COMP_WORDS[COMP_CWORD]}"
    local cmds="init build run clean test doctor info stats fmt lint scan bench bundle deploy watch archive license diff request env parallel completion"
    COMPREPLY=( $(compgen -W "${cmds}" -- "${cur}") )
}
complete -F _forge_completions forge
)";
}

void generateZsh() {
    std::cout << R"(
#compdef forge
_forge() {
    local -a commands
    commands=(
        'init:Initialize a new project'
        'build:Build project binaries'
        'run:Execute binaries or arbitrary commands'
        'clean:Clean build directory'
        'test:Run project test suite'
        'archive:Archive workspace source files'
        'license:Audit project license compliance'
        'diff:Inspect workspace changes'
        'request:Execute HTTP API requests'
        'env:Validate configuration and environment keys'
        'parallel:Run commands in parallel worker pool'
        'completion:Generate shell completion script'
    )
    _describe -t commands 'forge command' commands
}
_forge "$@"
)";
}

void generateFish() {
    std::cout << R"(
complete -c forge -f -a "init build run clean test doctor info stats fmt lint scan bench bundle deploy watch archive license diff request env parallel completion"
)";
}

} // anonymous namespace

int runCompletion(int argc, char* argv[]) {
    enableConsoleEncoding();

    if (argc < 3) {
        printCompletionHelp();
        return 1;
    }

    std::string shell = argv[2];

    if (shell == "powershell" || shell == "pwsh") {
        generatePowerShell();
    } else if (shell == "bash") {
        generateBash();
    } else if (shell == "zsh") {
        generateZsh();
    } else if (shell == "fish") {
        generateFish();
    } else {
        std::cerr << "  [!] Unsupported shell: " << shell << "\n";
        printCompletionHelp();
        return 1;
    }

    return 0;
}

} // namespace forge