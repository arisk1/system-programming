Katopodis Panagiotis Aristomenis


ergasia3:

perioexomena:
mirrorInitiator.c
mirrorServer.c
msFunctions.c
msFunctions.h
contentServer.c
csFunctions.c
csFunctions.h


kanei compile me thn entolh: make
svinei ta ektelesima kai .o kai me thn entolh: make clean.

Ektelesh:
prepei na ektelstoun prota oi contents servers kai o miror server me opoiadipote seira
kai telos o mirrorInitiator.

1)./contentServer -p port1 -d path // path einai to perioxomeno pou diathetei o content server gia antigrafh

mporoun na treksoun kai alloi content server.

2)./mirrorServer -p port2 -m path -w numOfWorkers // path einai to simeio pou apothikeuei o mirror server
ta arxeia pou antegrapse apo tous content servers

3)./mirrorInitiator -n hostname -p port2 -s requests // hostname einai to onoma 
tou mirror server px.linux02.di.uoa.gr , port2 einai to port pou kanei sindesi me to mirror server ,
kai request einai tis morfis px. linux03.di.uoa.gr:port1:fileordir:delay

>>MirrorInitiator:
kanei tcp sindesi me to mirror server kai gia na ksekinisei h efarmogh
kai na  stelnei ta requests sto mirror server prepei na stalei apo to mirror server h leksi start
epeita steleni ta requests kai perimenei 
1)ton arithmo arxeion pou metaferthikan
2)to sinoliko megethos ton byte pou metafethikan
3)to meso oro bytes pou metaferthikan  ( 1) / 2) )
molis ta labei kleinei th sindeisi kai termatizei.

>>mirrorServer:
kanei tcp sindesi me to mirror initiator kai steleni th leksh start gia na ksekinisei kai na labei
ta request.analoga me ta request ipologizei posa mirrormanager tha xreiastei na dimiourgisei kai dimiourgei
kai mia lista me ta stoixeia ton content server pou perixoun to onomna tous kai ton arithmo twn request pou zitise 
apo to kathena o mirrorinitiator,kai ena pinanka apo struct me ta stoixeia ton request. 
.kanoume create ta mirrormanager kai dinoume san orisma to struct me ta stoixeia tou request.
->>mirrorManager:kanei tcp sindesi me ton content server kai zitame to aithma LIST.
   kai kanoume write sto rounded buffer me th diadikasia lock/unlock mutex 	
->>worker:kanei read apo to rounded buffer me th diadikasia lock/unlock mutex.
   kanei tcp sindesi me tous content server kai stelnei to request Fetch
   sth sinexeia vlepei an auto pou zitaei apo ton content server einai file h dir kai pratei analoga.
   afou metaferthoun ta arxeia kai anevazei to numOfFilesTr kai to sumOfBytesSent kai meinetai kata 
   ena ta requests kai otan ftasei 0 anevazei th metablhth numDevicesDone kata ena pou einai h metablhth pou
   simatodotei to telos ths efarmoghs otan ginei iso me ton arithmo ton contentserver.
kai prin kleisei h efarmogh stelnei sto mirrorInitiator
1)ton arithmo arxeion pou metaferthikan
2)to sinoliko megethos ton byte pou metafethikan
3)to meso oro bytes pou metaferthikan  ( 1) / 2) )

>>contentServer
kanei tcp sindesi me to worker.kai perimenei ta tou stalei ena aitmhma LIST h FETCH.
to LIST steleni piso ena string me ta path apo to folder pou diathetei o content server
to Fetch analoga me to an eiani dir h arxeio steleni ta arxeia sto worker.


   
   
   