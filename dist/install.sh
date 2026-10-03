 #!/usr/bin/env bash
# Universal Installer for ESP32 Circuit Architect Plugin & Skill
# Compatible with Claude Code, OpenAI Codex, and Google Antigravity / Gemini CLI
set -eo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PLUGIN_SRC="${SCRIPT_DIR}/esp32-circuit-architect.plugin"
SKILL_SRC="${PLUGIN_SRC}/skills/esp32-circuit-architect"

echo "=========================================================="
echo "⚡ ESP32 Circuit Architect - Universal Installer v1.0.0"
echo "=========================================================="

install_claude() {
    echo "📦 Installing to Claude Code..."
    mkdir -p "${HOME}/.claude/skills/esp32-circuit-architect"
    mkdir -p "${HOME}/.claude/plugins/esp32-circuit-architect"
    cp -r "${SKILL_SRC}/"* "${HOME}/.claude/skills/esp32-circuit-architect/"
    cp -r "${PLUGIN_SRC}/"* "${HOME}/.claude/plugins/esp32-circuit-architect/"
    echo "  ✅ Installed to ~/.claude/skills/esp32-circuit-architect"
    echo "  ✅ Installed to ~/.claude/plugins/esp32-circuit-architect"
}

install_codex() {
    echo "📦 Installing to OpenAI Codex..."
    mkdir -p "${HOME}/.codex/skills/esp32-circuit-architect"
    mkdir -p "${HOME}/.codex/plugins/esp32-circuit-architect"
    cp -r "${SKILL_SRC}/"* "${HOME}/.codex/skills/esp32-circuit-architect/"
    cp -r "${PLUGIN_SRC}/"* "${HOME}/.codex/plugins/esp32-circuit-architect/"
    echo "  ✅ Installed to ~/.codex/skills/esp32-circuit-architect"
    echo "  ✅ Installed to ~/.codex/plugins/esp32-circuit-architect"
}

install_gemini() {
    echo "📦 Installing to Google Antigravity / Gemini CLI..."
    mkdir -p "${HOME}/.gemini/config/skills/esp32-circuit-architect"
    mkdir -p "${HOME}/.gemini/config/plugins/esp32-circuit-architect"
    cp -r "${SKILL_SRC}/"* "${HOME}/.gemini/config/skills/esp32-circuit-architect/"
    cp -r "${PLUGIN_SRC}/"* "${HOME}/.gemini/config/plugins/esp32-circuit-architect/"
    echo "  ✅ Installed to ~/.gemini/config/skills/esp32-circuit-architect"
    echo "  ✅ Installed to ~/.gemini/config/plugins/esp32-circuit-architect"
}

install_local() {
    echo "📦 Installing to current local repository (.agents/skills)..."
    mkdir -p ".agents/skills/esp32-circuit-architect"
    cp -r "${SKILL_SRC}/"* ".agents/skills/esp32-circuit-architect/"
    echo "  ✅ Installed to .agents/skills/esp32-circuit-architect"
}

case "${1:-}" in
    --claude)
        install_claude
        ;;
    --codex)
        install_codex
        ;;
    --gemini|--antigravity)
        install_gemini
        ;;
    --local)
        install_local
        ;;
    --all|"")
        if [ -d "${HOME}/.claude" ]; then install_claude; fi
        if [ -d "${HOME}/.codex" ]; then install_codex; fi
        if [ -d "${HOME}/.gemini" ]; then install_gemini; fi
        install_local
        ;;
    *)
        echo "Usage: $0 [--claude | --codex | --gemini | --local | --all]"
        exit 1
        ;;
esac

echo ""
echo "🎉 Installation Complete! Ready to use across your AI agents."
echo "=========================================================="
