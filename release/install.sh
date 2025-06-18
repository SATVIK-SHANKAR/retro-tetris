# releases/install.sh
#!/bin/bash
set -e

REPO="yourusername/retro-tetris"
OS="$(uname -s)"
ARCH="$(uname -m)"

echo "🎮 Installing Retro Terminal Tetris..."

case "$OS" in
    Linux*)  BINARY="tetris-linux-x64" ;;
    Darwin*) BINARY="tetris-macos-x64" ;;
    *)       echo "❌ Unsupported OS: $OS"; exit 1 ;;
esac

LATEST_URL="https://api.github.com/repos/$REPO/releases/latest"
DOWNLOAD_URL=$(curl -s $LATEST_URL | grep "browser_download_url.*$BINARY" | cut -d '"' -f 4)

if [ -z "$DOWNLOAD_URL" ]; then
    echo "❌ Could not find a release for your platform."
    exit 1
fi

curl -L -o tetris "$DOWNLOAD_URL"
chmod +x tetris

if [ -w /usr/local/bin ]; then
    mv tetris /usr/local/bin/
    echo "✅ Installed to /usr/local/bin/tetris"
else
    echo "✅ Downloaded to ./tetris"
    echo "💡 Run with: ./tetris"
    echo "💡 Or move to PATH: sudo mv tetris /usr/local/bin/"
fi

echo "🎉 Installation complete! Run 'tetris' to play!"
