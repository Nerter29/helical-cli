# Descritpion
This program is a helical solar system visualisation, by representing the planets and their trails with ascii characters in the terminal.

# Installation
### Linux is required


#### Option 1 : Run the executable :
```
git clone https://github.com/Nerter29/helical-cli.git
cd helical-cli
chmod +x helical
./helical
```
#### Option 2 : Compile it yourself (gcc required) :
```
git clone https://github.com/Nerter29/helical-cli.git
cd helical-cli
g++ main.cpp body.h body.cpp helical.cpp helical.h -o your-helical
./your-helical
```
#### Option 3 : If you are on Arch Linux :
```
yay -Syu helical-cli-git
helical
```
