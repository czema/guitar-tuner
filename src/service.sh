#!/bin/bash
if [[ "$EUID" -ne 0 ]]; then
	echo -e "\033[1;31mRoot access required.\033[0m" 2>&1
	exit 1
fi

echo -e "\033[1;33mInstalling service...\033[0m"
systemctl stop guitar-tuner.service
systemctl disable guitar-tuner.service
cp guitar-tuner.service /lib/systemd/system/
systemctl enable guitar-tuner.service
echo -e "\033[0;32mDone...\033[0m"