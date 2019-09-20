Katopodis Panagiotis Aristomenis

ergasia2:

periexomena:

-coord.c
-console.c
-functions.c
-functions.h
-Makefile
-README

Apo thn ergasia den ilopoihthike:
-timeduration- status all
-resume
-suspend
-script

kanei complile me thn entolh : make
svinei ta ektelesima kai .o : make clan

trexei me tis entoles :
1) ./coord -l "path" -w "jmsout" -r "jmsin" -n jobspool
(to path prepei na iparxei ston ipologisti)

2)./console -w "jmsin" -r "jmsout" -o <arxeio me entoles> (proairetiko)


to protokollo pou xrisimopoieitai einai to eksis:
non blocking epikoinonia 
stelnei o enas ston allon ena string me to mikos ths entolhs
pairnoume auto to mikos me atoi()
an h entolh pou esteile o coord sto console einai SEND tote stelnei thn epomenh entolh
kai sthn sinexeia pairnoume thn entolh kai tin epekserasomaste analoga.
h entolh steleni apanthshs prote ena string me to mikos thns grammis kai meta thns enotlh 
an o console diabasei mia enntolh pou tou estile o coords kai den einai send tote einai apanthsh 
apo ton coord.o coord sto telos kathe apathshs bazei to string  0 gia na pei pos teliose h apanthsh
sthn anamonh o coord mporei na labei apo ta pool mono to -5 pou simianei pos theloun na termatisoun.
An o coord dexthei shutdown tote stelei se ola ta pool tou SIGTERM kai perimenei me waitpid na 
termatisoun . kai girnaei sto coord o opoios stelinei sto console thn enolth SHUTDOWN kai termatizei 
kai o console me th seira tou kanei close ta pipe pou sindeoun console-coord kai termaizei kai autos.
O coord kanei ingnore to SIGPIPE kai to pool epeksergazetai to SIGTERM allazontas ena flag me mia sinartisi f.
gia ta pool:arxika kaoun fork() kai to paidi sti sinexeia kanei auto fork() kai dup2 kai execvp gia na treksoun
thn entolh submit pou dothike kai kapos antoistixa gia is alles enotles.
OI plhrofories gia ta pools kai jobs apothikeontai se listes.


