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


## port range allowed check in main
Character encoding rules must be followed for unsafe and reserved characters. Clients should take care that attempts to connect to ports other than 194 in the well known port range 1-1024, are disregarded. IRC servers often use the non-registered port 6667 (or ports in the range 6000-7000) for clients to connect to.
The de facto standard has always been to run IRC on 6667/TCP and nearby port numbers (for example TCP ports 6660–6669, 7000) to avoid having to run the IRCd software with root privileges. 

## /help
Use "/help <command>" for help on a specific command.
The following commands are available in this context:
action, authserv, away, chanserv, clear, clearall, ctcp, debug, deop, devoice, help, invite, j, join, kick, list, me, me, memoserv, mode, msg, names, nick, nickserv, notice, op, operserv, operwall, part, ping, query, quit, quote, remove, say, time, topic, umode, version, voice, wallops, whois, whowas.



## Handling suspended terminal

On MacOs executed with ctrl+z, testing on regular activity after suspending terminal
Find correspondent key combination in Linux

## Handling ctrl-D in nc terminal

Check the functionality of the program with ctrl-D interrupting the commands or any entry
Chck if multiple ctrl-D freeze the nc session also in Mac


## Actual todo list
- Adjust file transfer for server  TEST!!!!!! and clean
- File Cleanup 
- Testing
- README.md Namespace - check what possible with gif 
- Evaluate
- get outstanding, cause we deserve it!
