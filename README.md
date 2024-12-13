**RESTServerClient**

REST-Server und der dazugehörige Client.

Der Server ist in Python erstellt, und der Client in C++.

Als Usecase wird von einer Autolackiererei ausgegangen. Ein Auto wird registriert mittels seiner folgenden Eigenschaften: Hersteller, Modell, Farbe. Beim Eintrag des Autos in die Datenbank kriegt es eine ID. Basierend auf dieser ID kann dann die Autofarbe mittels eines Update-Requests geändert werden.

**Benutzte Komponenten**

Server:
* FastAPI (https://fastapi.tiangolo.com)
* SQLModel (https://sqlmodel.tiangolo.com)

Client:
* C++ Requests (https://github.com/libcpr/cpr)
* JSON for Modern C++ (https://github.com/nlohmann/json)

**Systemvoraussetzungen**

Folgende Komponenten müssen auf dem System installiert sein:
* Python (Mindestversion 3.10.x)
* CMake (Mindestversion 3.20)
* Aktueller C++ Compiler (GCC und Visual Studio werden unterstützt)
* Git für den Download des Repos (oder Download-Funktion der Github-Seite nutzen)

**Installation der benötigten Python-Module**

Damit der Server lauffähig ist, müssen die benötigten Python-Module zuerst installiert werden. In der Konsole eingeben:

```
pip install "fastapi[standard]"
pip install sqlmodel
```
**Starten des Servers und Kompilierung des Clients**

Neues Verzeichnis auf dem Computer erstellen, Kommandozeile in dem Verzeichnis öffnen und Repo klonen:

```
git clone https://github.com/ArseniJachontow/RESTServerClient
```

Server starten:

```
cd RESTServerClient
cd Server
python -m fastapi dev server.py
```
Der Server gibt aus, unter welcher Adresse er erreichbar ist (zum Beispiel: http://127.0.0.1:8000), diese Adresse rauskopieren, da sie für den Client gebraucht wird.

Client kompilieren:

Es wird CMake für die Kompilierung des Clients benutzt. Die nötige Konfigurationsdatei ist im Repo enthalten und muss nicht modifziert werden.

Neues Kommandozeilenfenster öffnen und in das Verzeichnis navigieren, wohin das Repo geklont wurde, dann:

```
cd RESTServerClient
cd Client
cd build
cmake ..
cmake --build .
```

**Benutzeranleitung des Clients**

Es können folgende Kommandos benutzt werden:

* -create_car [Hersteller Modell Farbe]
* -read_all
* -read_car [ID]
* -update_car [ID]
* -delete_car [ID]

Das erste Argument des Clients ist immer die Adresse des Servers, in den Beispielen hier http://127.0.0.1:8000, bitte ändern, falls diese abweicht.

Nach der Kompilierung befindet sich der Client im Verzeichnis **RESTServerClient\Client\build\Debug**

Ins Verzeichnis navigieren, Kommandozeile dort öffnen und Kommandos ausprobieren:

```
./client http://127.0.0.1:8000 -create_car Ford Explorer Red
./client http://127.0.0.1:8000 -create_car Toyota Corolla Blue
./client http://127.0.0.1:8000 -create_car VW ID_Buzz Purple

./client http://127.0.0.1:8000 -read_all

./client http://127.0.0.1:8000 -read_car 1

./client http://127.0.0.1:8000 -update_car 1 Orange

./client http://127.0.0.1:8000 -delete_car 1
```
