# pavelock
A FreeBSD Jails management system

## Installation
1. Build the Pavelock program source.

`$ make`

2. Build and install the Pavelock shell (plsh) to /tmp/plsh (setting sbit to run as root)

`$ make plsh`

3. Add a new user called 'pavelock'

`# adduser`

4. Set the shell of 'pavelock' to '/bin/plsh'

`# chsh pavelock`

5. Ensure that the default "open" firewall (IPFW) and nat are running

```
# echo 'firewall_enable="YES"' >> /etc/rc.conf
# echo 'firewall_type="open"' >> /etc/rc.conf
# echo 'firewall_nat_enable="YES"' >> /etc/rc.conf
```

## Usage
SSH into the server using the 'pavelock' user.

`$ ssh pavelock@myhost.my.domain`

You will be greeted with the following screen:

    *****************************************************
    *    ___                      _               _     *
    *   / _ \  __ _ __   __  ___ | |  ___    ___ | | __ *
    *  / /_)/ / _` |\ \ / / / _ \| | / _ \  / __|| |/ / *
    * / ___/ | (_| | \ V / |  __/| || (_) || (__ |   <  *
    * \/      \__,_|  \_/   \___||_| \___/  \___||_|\_\ *
    *                powered by FreeBSD                 *
    *****************************************************
    
    Would you like to login or register?
    
    1) Login
    2) Register
    
    0) Exit
    
    Choice:

Once you have registered a new user, you can log in and will be able to access the main Jail menu.

    *****************************************************
    *    ___                      _               _     *
    *   / _ \  __ _ __   __  ___ | |  ___    ___ | | __ *
    *  / /_)/ / _` |\ \ / / / _ \| | / _ \  / __|| |/ / *
    * / ___/ | (_| | \ V / |  __/| || (_) || (__ |   <  *
    * \/      \__,_|  \_/   \___||_| \___/  \___||_|\_\ *
    *                powered by FreeBSD                 *
    *****************************************************
    Welcome, what would you like to do?
    
    1) Attach to running Jail
    2) Start/stop existing Jails
    3) Create new Jail
    4) Delete existing Jail
    5) Modify account information
    
    0) Exit
    
    Choice:
