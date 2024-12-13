**RestServerClient**

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
