DIR=$PWD

echo -e "Running Setup in ${PWD}"
echo -e "\n\tapt updating...\n"

sudo apt update

echo -e "\n\tInstalling Dependencies...\n"

xargs sudo apt install <dependencies.txt -y
pip3 install -r python3_requirements.txt

echo -e "\n\tExtracting gcc...\n"

tar -xvf ${HOME}/Downloads/gcc-arm-none-eabi-10-2020-q4-update-x86_64-linux.tar.bz2 -C ~/

echo -e "\n\tStaching Cheats...\n"
echo "export PATH=${HOME}/gcc-arm-none-eabi-10-2020-q4-major/bin:${HOME}/.local/bin:${PATH}" >> ${HOME}/.bashrc

echo -e "\n\tSetting your PATH...\n"
export PATH=${HOME}/gcc-arm-none-eabi-10-2020-q4-major/bin:${HOME}/.local/bin:${PATH}

source /etc/os-release
if [[ "${VERSION}" == "18.04.5 LTS (Bionic Beaver)" ]]; then

	echo -e "\n\t18.04.5 LTS (Bionic Beaver) Detected adding alias\n"
	echo 'alias scons="/usr/bin/env python3 $(which scons)"' >> ${HOME}/.bashrc
fi

cd ${HOME}/

echo -e "\n\tSetting up openocd...\n"
git clone https://github.com/openocd-org/openocd.git
cd openocd && ./bootstrap && ./configure && make && sudo make install

echo -e "\n\tSetting up STM32 and STLink...\n"
cd ${HOME}
mkdir stm32
cd stm32
git clone https://github.com/texane/stlink
cd stlink
cmake .
make
sudo make install
cd bin
sudo cp st-* /usr/local/bin
cd ..
sudo cp config/udev/rules.d/49-stlinkv* /etc/udev/rules.d
sudo udevadm control --reload
cd ${DIR}/..

echo -e "\n\tInitializing submodules...\n"
git submodule update --init --recursive

# git remote add template https://gitlab.com/aruw/controls/taproot-template-project.git
# git pull template develop
# git push --set-upstream origin master

echo -e "\nSetup Finished!"

