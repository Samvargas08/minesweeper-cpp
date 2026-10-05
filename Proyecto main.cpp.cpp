#include <iostream>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace std;

const int MAX = 12;

struct Jugador {

    string nombre;
    int victorias;
    int derrotas;
    int monedas;
};

void inicializar(char tablero[MAX][MAX],
                 int filas,
                 int columnas) {

    for (int i = 0; i < filas; i++) {

        for (int j = 0; j < columnas; j++) {

            tablero[i][j] = '-';
        }
    }
}


void generarMinas(char minas[MAX][MAX],
                  int filas,
                  int columnas,
                  int cantidad) {

    inicializar(minas, filas, columnas);

    int contador = 0;

    while (contador < cantidad) {

        int x = rand() % filas;
        int y = rand() % columnas;

        if (minas[x][y] != '*') {

            minas[x][y] = '*';
            contador++;
        }
    }
}

void mostrar(char tablero[MAX][MAX],
             int filas,
             int columnas) {

    cout << endl;
    cout << "    ";

    for (int j = 0; j < columnas; j++) {
        if (j < 10)
            cout << " " << j << "  ";
        else
            cout << j << "  ";
    }

    cout << endl << endl;

    for (int i = 0; i < filas; i++) {

        cout << setw(2) << i << " ";

        for (int j = 0; j < columnas; j++) {
            cout << "[" << tablero[i][j] << "] ";
        }

        cout << endl;
    }
}

int contarMinas(char minas[MAX][MAX],
                int filas,
                int columnas,
                int x,
                int y) {

    int contador = 0;

    for (int i = x - 1; i <= x + 1; i++) {

        for (int j = y - 1; j <= y + 1; j++) {

            if (i >= 0 && i < filas &&
                j >= 0 && j < columnas) {

                if (minas[i][j] == '*') {

                    contador++;
                }
            }
        }
    }

    return contador;
}

void revelar(char tablero[MAX][MAX],
             char minas[MAX][MAX],
             bool visitado[MAX][MAX],
             int filas,
             int columnas,
             int x,
             int y) {

    if (x < 0 || x >= filas ||
        y < 0 || y >= columnas)
        return;

    if (visitado[x][y])
        return;

    if (tablero[x][y] == 'F')
        return;

    visitado[x][y] = true;

    int minasCerca =
        contarMinas(minas,
                    filas,
                    columnas,
                    x,
                    y);

    tablero[x][y] = minasCerca + '0';

    if (minasCerca == 0) {

        tablero[x][y] = '0';

        for (int i = x - 1; i <= x + 1; i++) {

            for (int j = y - 1; j <= y + 1; j++) {

                revelar(tablero,
                         minas,
                         visitado,
                         filas,
                         columnas,
                         i,
                         j);
            }
        }
    }
}

bool victoria(bool visitado[MAX][MAX],
              char minas[MAX][MAX],
              int filas,
              int columnas,
              int cantidadMinas) {

    int descubiertas = 0;

    for (int i = 0; i < filas; i++) {

        for (int j = 0; j < columnas; j++) {

            if (visitado[i][j]) {

                descubiertas++;
            }
        }
    }

    return descubiertas ==
           (filas * columnas - cantidadMinas);
}

void radar(char tablero[MAX][MAX],
           char minas[MAX][MAX],
           bool visitado[MAX][MAX],
           int filas,
           int columnas) {

    for (int i = 0; i < filas; i++) {

        for (int j = 0; j < columnas; j++) {

            if (minas[i][j] != '*' &&
                !visitado[i][j]) {

                revelar(tablero,
                         minas,
                         visitado,
                         filas,
                         columnas,
                         i,
                         j);

                cout << "\nRADAR ACTIVADO!\n";
                return;
            }
        }
    }
}

void guardarRanking(Jugador jugador) {

    ofstream archivo("ranking.txt", ios::app);

    archivo << jugador.nombre << " "
            << jugador.victorias << " "
            << jugador.derrotas << " "
            << jugador.monedas << endl;


    cout << "\nNo hay jugadores registrados todavia.\n";
    archivo.close();
}

void mostrarRanking() {

    ifstream archivo("ranking.txt");

    string nombre;
    int v, d, m;

    cout << "\n======= RANKING =======\n";

    if (!archivo){
        cout << "\nNo hay jugadores registrados todavia.\n";
        return;
    }

    bool hayDatos=false;
    while (archivo >> nombre >> v >> d >> m) {
        hayDatos=true;

        cout << "\nJugador: " << nombre << endl;
        cout << "Victorias: " << v << endl;
        cout << "Derrotas: " << d << endl;
        cout << "Monedas: " << m << endl;
    }

    archivo.close();
}

void jugar(Jugador &jugador) {

    int opcionDif;

    cout << "\n===== DIFICULTAD =====\n";
    cout << "1. Facil\n";
    cout << "2. Medio\n";
    cout << "3. Dificil\n";
    cout << "Seleccione: ";

    cin >> opcionDif;

    int filas, columnas, minasCantidad;

    switch(opcionDif) {

        case 1:
            filas = columnas = 8;
            minasCantidad = 10;
            break;

        case 2:
            filas = columnas = 10;
            minasCantidad = 20;
            break;

        case 3:
            filas = columnas = 12;
            minasCantidad = 30;
            break;

        default:
            cout << "\nOpcion invalida.\n";
            return;
    }

    char tablero[MAX][MAX];
    char minas[MAX][MAX];

    bool visitado[MAX][MAX] = {false};

    inicializar(tablero, filas, columnas);
    generarMinas(minas, filas, columnas, minasCantidad);

    bool terminar = false;
    bool segundaVida = true;

    while (!terminar) {

        cout << "\nMonedas: "
             << jugador.monedas << endl;

        mostrar(tablero, filas, columnas);

        cout << "\n===== ACCIONES =====\n";
        cout << "1. Descubrir casilla\n";
        cout << "2. Usar radar (5 monedas)\n";
        cout << "3. Colocar bandera\n";
        cout << "4. Ver estadisticas\n";
        cout << "5. Salir\n";

        int opcion;
        cin >> opcion;

        int x, y;

        switch(opcion) {

            case 1:

                cout << "\nFila: ";
                cin >> x;

                if (!cin) {

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\nValor invalido, tienes que ingresar un numero\n";

                    break;
                }

                cout << "Columna: ";
                cin >> y;

                if (!cin) {

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\nValor invalido, tienes que ingresar un numero\n";

                    break;
                }

                if (x < 0 || x >= filas ||
                    y < 0 || y >= columnas) {

                    cout << "\nPosicion invalida.\n";
                    break;
                }

                if (visitado[x][y]) { cout<<"\nCasilla ya descubierta.\n"; break; }
                if (tablero[x][y] == 'F') {

                    cout << "\nCasilla con bandera.\n";
                    break;
                }

                if (minas[x][y] == '*') {

                    if (segundaVida) {

                        segundaVida = false;

                        cout << "\nSEGUNDA VIDA ACTIVADA!\n";

                        minas[x][y]='-';
                        revelar(tablero,minas,visitado,filas,columnas,x,y);
                        break;
                    }

                    cout << "\nBOOM! PERDISTE\n";

                    jugador.derrotas++;

                    for (int i = 0; i < filas; i++) {

                        for (int j = 0; j < columnas; j++) {

                            if (minas[i][j] == '*') {

                                tablero[i][j] = '*';
                            }
                        }
                    }

                    mostrar(tablero, filas, columnas);

                    terminar = true;

                    break;
                }

                revelar(tablero,
                         minas,
                         visitado,
                         filas,
                         columnas,
                         x,
                         y);

                if (victoria(visitado,
                             minas,
                             filas,
                             columnas,
                             minasCantidad)) {

                    for (int i=0;i<filas;i++) for(int j=0;j<columnas;j++) if(minas[i][j]=='*') tablero[i][j]='*';
                    mostrar(tablero,filas,columnas);
                    cout << "\nFELICIDADES! GANASTE!\n";

                    jugador.victorias++;

                    jugador.monedas += 15;

                    cout << "\nGanaste 15 monedas!\n";

                    terminar = true;
                }

                break;

            case 2:

                if (jugador.monedas >= 5) {

                    jugador.monedas -= 5;

                    radar(tablero,
                           minas,
                           visitado,
                           filas,
                           columnas);
                }
                else {

                    cout << "\nNo tienes monedas.\n";
                }

                break;

            case 3:

                cout << "\nFila: ";
                cin >> x;

                if (!cin) {

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\nValor invalido, tienes que ingresar un numero\n";

                    break;
                }

                cout << "Columna: ";
                cin >> y;

                if (!cin) {

                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "\nValor invalido, tienes que ingresar un numero\n";

                    break;
                }

                if (x >= 0 && x < filas &&
                    y >= 0 && y < columnas) {

                    if(!visitado[x][y]){ if(tablero[x][y]=='F') tablero[x][y]='-'; else tablero[x][y]='F'; }
                }

                break;

            case 4:

                cout << "\n===== ESTADISTICAS =====\n";

                cout << "Jugador: "
                     << jugador.nombre << endl;

                cout << "Victorias: "
                     << jugador.victorias << endl;

                cout << "Derrotas: "
                     << jugador.derrotas << endl;

                cout << "Monedas: "
                     << jugador.monedas << endl;

                break;

            case 5:

                cout << "\nSaliendo partida...\n";
                return;

            default:

                cout << "\nOpcion invalida.\n";
        }
    }

    guardarRanking(jugador);
}

int main() {

    srand(time(0));

    Jugador jugador;

    jugador.victorias = 0;
    jugador.derrotas = 0;
    jugador.monedas = 20;

    cout << "====================================\n";
    cout << "              MINEFIELD             \n";
    cout << "====================================\n";

    cout << "\nIngrese su nombre: ";
    cin >> jugador.nombre;

    int opcion;

    do {

        cout << "\n========== MENU ==========\n";
        cout << "1. Jugar\n";
        cout << "2. Ver ranking\n";
        cout << "3. Salir\n";

        cout << "Seleccione: ";
        cin >> opcion;

        if (!cin) {

            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nValor invalido, tienes que ingresar un numero\n";


            continue;

        }

        switch(opcion) {

            case 1:
                jugar(jugador);
                break;

            case 2:
                mostrarRanking();
                break;

            case 3:
                cout << "\nGracias por jugar!\n";
                break;

            default:
                cout << "\nOpcion invalida\n";
        }

    } while(opcion != 3);

    return 0;
}


