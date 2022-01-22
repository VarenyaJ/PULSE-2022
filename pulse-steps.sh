cd && pwd
sudo apt-get update -y && sudo apt-get upgrade -y
sudo apt-get install vim && sudo apt install git-all
sudo apt-get install -y build-essential tk-dev libncurses5-dev libncursesw5-dev libreadline6-dev libdb5.3-dev libgdbm-dev libsqlite3-dev libssl-dev libbz2-dev libexpat1-dev liblzma-dev zlib1g-dev libffi-dev tar wget vim
python3 --version
sudo apt-get update -y && sudo apt-get upgrade -y

cd && git clone https://bitbucket.org/sol_prog/raspberry-pi-gcc-binary.git
cd raspberry-pi-gcc-binary
tar -xjvf gcc-10.1.0-armhf-raspbian.tar.bz2
sudo mv gcc-10.1.0 /opt
cd ..
#rm -rf raspberry-pi-gcc-binary

gcc-10.1 --version

