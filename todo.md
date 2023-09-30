#	Figuring out functions
Sockets (big)		
Network Byte		
Internet Address
Send/Receive						Emanuela			claimed
Signal Handling						Minitalk/Minishell	we should know this
File Functions		
Polling Functions	branch:Polling	Orlando				works

##  Channel Commands 

∗ INVITE - Invite a client to a channel (Done?)
∗ TOPIC - Change or view the channel topic (Done?)
∗ KICK - Eject a client from the channel
∗ MODE - Change the channel’s mode:
    · i: Set/remove Invite-only channel
    · t: Set/remove the restrictions of the TOPIC command to channel operators
    · k: Set/remove the channel key (password)
    · o: Give/take channel operator privilege
    · l: Set/remove the user limit to channel

## Handling suspended terminal

On MacOs executed with ctrl+z, testing on regular activity after suspending terminal
Find correspondent key combination in Linux


## File transfer
file transfers in IRC clients like Libera and Pidgin are typically facilitated using the PRIVMSG command.
This aligns with the standard practice in IRC for sending files and private messages.

Implementing file transfer functionality in your IRC server, focusing on handling file transfers through PRIVMSG is a reasonable approach. It ensures compatibility with a wide range of IRC clients and follows established conventions.
The protocol snippet you've provided is indeed an example of a DCC (Direct Client-to-Client) file transfer request initiated via a PRIVMSG command. Let's break down this protocol message:

PRIVMSG MagicEma :DCC SEND "file.doc" 168493318 41107 6

    PRIVMSG MagicEma: In this context, it's used to initiate a DCC file transfer with this specific recipient.

    :DCC SEND "file.doc" 168493318 41107 6: This is the custom DCC file transfer request within the PRIVMSG message.
    
        DCC SEND: This part of the message indicates that a DCC file transfer is requested.

        "file.doc": This is the name of the file being sent, in this case, "file.doc." The file name is enclosed in double quotes for clarity.

        168493318: This number represents the file size in bytes. It indicates that the file being sent is 168493318 bytes (approximately 168.49 MB) in size.

        41107: This number represents the IP address of the sender in hexadecimal format.

        6: This number represents the port number for the DCC connection.

In summary, this protocol message is a request from the sender to the recipient to initiate a DCC file transfer of a file named "file.doc" with a size of approximately 168.49 MB. It specifies the sender's IP address and the port number for the DCC connection. The recipient's client is expected to respond to this request to establish the file transfer connection and retrieve the file.

## port range allowed check in main
Character encoding rules must be followed for unsafe and reserved characters. Clients should take care that attempts to connect to ports other than 194 in the well known port range 1-1024, are disregarded. IRC servers often use the non-registered port 6667 (or ports in the range 6000-7000) for clients to connect to.
The de facto standard has always been to run IRC on 6667/TCP and nearby port numbers (for example TCP ports 6660–6669, 7000) to avoid having to run the IRCd software with root privileges. 

## /help
Use "/help <command>" for help on a specific command.
The following commands are available in this context:
action, authserv, away, chanserv, clear, clearall, ctcp, debug, deop, devoice, help, invite, j, join, kick, list, me, me, memoserv, mode, msg, names, nick, nickserv, notice, op, operserv, operwall, part, ping, query, quit, quote, remove, say, time, topic, umode, version, voice, wallops, whois, whowas.
