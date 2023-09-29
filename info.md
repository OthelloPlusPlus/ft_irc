# Links

##	Potential clients
https://libera.chat/guides/clients
 - https://pidgin.im/ (Windows, Linux, and other UNIX-like operating systems)
	brew install -v pidgin
 - https://adium.im/ (Pidgin for macOS)

##	Protocols documentation
https://modern.ircdocs.horse/#client-to-server-protocol-structure

https://datatracker.ietf.org/doc/html/rfc1459

https://beej.us/guide/bgnet/html/

http://chi.cs.uchicago.edu/chirc/index.html

https://modern.ircdocs.horse/dcc.html

## Eval 
https://rphlr.github.io/42-Evals/Rank05/ft_irc/

##  Pidgin
rm -rf ~/.purple
rm -rf ~/.config/pidgin
rm -rf ~/pidgin
which pidgin (get where is the binary)
cd /home....
rm pidgin

mkdir homebrew && curl -L https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C homebrew
echo 'export PATH="$HOME/homebrew/bin:$PATH"' >> ~/.zshrc 
brew install pidgin

misisng modules

brew install at-spi2-atk
brew install libcanberra-gtk-module
brew list
dpkg -l | grep libgtk   

I still receive the warning on terminal, 
I also added the new modules in PATH

export PKG_CONFIG_PATH=$HOME/.homebrew/lib/pkgconfig
export LD_LIBRARY_PATH=$HOME/.homebrew/lib
export GTK_PATH=$HOME/.homebrew/lib/gtk-2.0

error 	C_LRED text + C_RESET variables + C_LRED rest of the text
message C_RESET text + C_LCYAN variables + C_RESET rest of the text
Joining C_RESET text + C_CYAN variables + C_RESET rest of the text
leaving C_RED text + C_RESET variables + C_RED rest of the text

sendMsg --> never forget \r\n, or never forgive

## port
IRC
In the context of IRC (Internet Relay Chat), the default and commonly used port for IRC communication is 6667 for plaintext connections and 6697 for SSL/TLS-encrypted connections. These ports are part of the "well-known ports" range, specifically within the "registered ports" range (ports 1024 to 49151) mentioned earlier.

DCC
In the DCC (Direct Client-to-Client) protocol for file transfers, the port numbers used for the direct connections are typically in the range of 1024 to 65535. This range is often referred to as "ephemeral" or "private" ports. Ports in this range are commonly used for various network services, including DCC file transfers, because they are less likely to be reserved for specific well-known services.


## verbose format
error 	C_LRED text + C_RESET variables + C_LRED rest of the text
message C_RESET text + C_LCYAN variables + C_RESET rest of the text
Joining C_RESET text + C_CYAN variables + C_RESET rest of the text
leaving C_RED text + C_RESET variables + C_RED rest of the text

sendMsg --> never forget \r\n, or never forgive

## EWOULDBLOCK & EAGAIN

EWOULDBLOCK & EAGAIN - typically used to indicate that a non-blocking socket operation 
would block because there is no data available to read at the moment. 
In other words, these error codes mean that the recv function didn't receive any data 
because the socket is non-blocking and no data was immediately available.
