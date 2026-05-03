# Come compilare
Dopo aver modificato CMakeList.txt

- cd ~/GIT/tesi-neural-amp
- cmake -B build -G "Unix Makefiles"
- cmake --build build --config Debug -j 4

Dopo aver modificato i file .cpp o .h

- cd ~/GIT/tesi-neural-amp
- cmake --build build --config Debug -j 4

Per ripartire da una Build pulita usando 4 core in parallelo

- cd ~/GIT/tesi-neural-amp
- rm -rf build
- cmake -B build -G "Unix Makefiles"
- cmake --build build --config Debug -j 4

Lanciare lo Standalone per test:

- open build/NeuralAmp_artefacts/Debug/Standalone/NeuralAmp.app

In caso di fallimento della build posso leggere solo gli errori:
- cmake --build build --config Debug -j 4 2>&1 | grep "error:"

# Compito 1
In questo compito dovevo creare una funzione che visualizzasse un gain nella finestra del plugin.
## Cosa ho imparato
- Ho imparato dove inserire le funzioni e per quale motivo vanno nel Processor piuttosto che nell'editor. 
- Ho capito il funzionamento di AudioProcessorValueTreeState
- Ho capito come si chiamano le classi annidate
- Ho imparato a restituire il ParameterLayout passando una lista di unique_ptr, responsabili di gestire un oggetto tramite il pointer
- Ho imparato a creare dei parametri con identificatori univoci, sfruttando i puntatori, make_unique e getRawParameterValue
- Ho imparato a modificare il processBlock per eseguire le operazioni che voglio sul suono in ingresso
- Ho imparato a leggere e creare dei puntatori atomici
- Ho imparato a gestire AudioBuffer ricevuto nel processBlock 
- Imparato a gestire la posizione dei components
- Imparato a creare uno slider base rotativo
## Errori e difficoltà incontrate
- Non riuscivo a capire come andassero creati i parametri
- Non riuscivo a leggere i puntatori atomici
- Fatico a capire quali membri vanno messi come private perché vengono usati solo dall'eitor o dal processor
- Confusione tra initializer list e corpo del costruttore
## Domande aperte
- Perché devono essere degli uninque pointer? A che servono? Perché devo usare AudioParameterFloat?
- Non mi è ben chiaro come funzionano i costruttori in C++
- Cosa sono esattamente i puntatori atomici, che differenza fa tra questi e quelli normali
- Come fa ad arrivare il buffer audio nel processBlock? Chi lo chiama mettendogli il buffer audio come parametro?
- Cosa sono gli unique_ptr
- Quando initializer list e quando scrivo nel corpo del costruttore? Perché?
