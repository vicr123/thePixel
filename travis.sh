if [ $STAGE = "script" ]; then
  if [ $TRAVIS_OS_NAME = "linux" ]; then
    source /opt/qt510/bin/qt510-env.sh
    git clone https://github.com/vicr123/the-libs.git
    cd the-libs
    qmake
    make
    sudo make install INSTALL_ROOT=/
    cd ..
    qmake
    make
    make install INSTALL_ROOT=~/appdir
    cd ~
    wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
    wget -c -nv "https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage"
    sudo cp appimagetool-x86_64.AppImage /usr/bin/appimagetool
    sudo chmod +x /usr/bin/appimagetool
    chmod a+x linuxdeployqt-continuous-x86_64.AppImage
    ./linuxdeployqt-continuous-x86_64.AppImage appdir/usr/share/applications/thepixel.desktop -bundle-non-qt-libs
    ./linuxdeployqt-continuous-x86_64.AppImage appdir/usr/share/applications/thepixel.desktop -appimage
  else
    exit 1
    #echo "[TRAVIS] Building for macOS"
    #export PATH="/usr/local/opt/qt/bin:$PATH"
    #cd ..
    #mkdir "build-theterminal"
    #cd "build-theterminal"
    #echo "[TRAVIS] Running qmake"
    #qmake "INCLUDEPATH += /usr/local/opt/qt/include" "LIBS += -L/usr/local/opt/qt/lib" ../theTerminal/theTerminal.pro
    #echo "[TRAVIS] Building project"
    #make
    #echo "[TRAVIS] Deploying Qt Libraries"
    #macdeployqt theTerminal.app
    #echo "[TRAVIS] Preparing Disk Image creator"
    #npm install appdmg
    #echo "[TRAVIS] Building Disk Image"
    #./node_modules/appdmg/bin/appdmg.js ./node-appdmg-config.json ~/theTerminal-macOS.dmg
  fi
elif [ $STAGE = "before_install" ]; then
  if [ $TRAVIS_OS_NAME = "linux" ]; then
    wget -O ~/vicr12345.gpg.key https://vicr123.com/repo/apt/vicr12345.gpg.key
    sudo apt-key add ~/vicr12345.gpg.key
    sudo add-apt-repository 'deb https://vicr123.com/repo/apt/ubuntu bionic main'
    sudo add-apt-repository -y ppa:beineri/opt-qt-5.10.0-xenial
    sudo apt-get update -qq
    sudo apt-get install qt510-meta-minimal qt510x11extras qt510tools qt510translations libmount-dev xorg-dev libxcb-util0-dev libgl1-mesa-dev
  else
    exit 1
    echo "[TRAVIS] Preparing to build for macOS"
    brew update
    brew install qt5
  fi
elif [ $STAGE = "after_success" ]; then
  if [ $TRAVIS_OS_NAME = "linux" ]; then
    echo "[TRAVIS] Publishing AppImage"
    cd ~
    wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
    bash upload.sh thePixel*.AppImage*
  else
    echo "hello :)"
    exit 1
    #echo "[TRAVIS] Publishing Disk Image"
    #cd ~
    #wget -c https://github.com/probonopd/uploadtool/raw/master/upload.sh
    #bash upload.sh theTerminal-macOS.dmg
  fi
fi
