#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
#include <random>
#include <fstream>

int main() {
	ofstream ficheroSalida;
	ficheroSalida.open("probExt2Generador.pddl");
	ficheroSalida << "(define (problem planificacion-contenidos-ext2)" << endl; 
	ficheroSalida << "  (:domain planificacion-contenidos)" << endl;
	ficheroSalida << endl;
	ficheroSalida << "	(:objects " << endl;

	// CONTENIDO
	std::random_device rd;
	std::mt19937 eng(rd()); 
	std::uniform_int_distribution<> distr(3, 15); 
	int numContenido = distr(eng);  
	vector<string> nombresContenido (numContenido);
	ficheroSalida << "    ";
	for(int i = 0; i < numContenido; ++i) {
    	string contenido = "contenido";
    	nombresContenido[i] = contenido.append(to_string(i + 1));
    	ficheroSalida << nombresContenido[i] << " ";	
	}	
	ficheroSalida << "- contenido" << endl;

	// DIAS
	vector<string> nombresDias (31);
	ficheroSalida << "    ";
	for(int i = 0; i < nombresDias.size(); ++i) {
    	string dia = "dia";
    	nombresDias[i] = dia.append(to_string(i + 1));
    	ficheroSalida << nombresDias[i] << " ";	
	}
	ficheroSalida << "- dia" << endl;
	ficheroSalida << "  )" << endl << endl;

	// INIT
	ficheroSalida << "  (:init" << endl;

    // Predecesores
    std::uniform_int_distribution<> distrPred(2, numContenido - 1); 
    int numPredSeleccionados = distrPred(eng); // Número de contenidos con predecesores
    std::vector<bool> contenidoPredSelec(numContenido, false);

    int predSeleccionados = 0;
    while (predSeleccionados < numPredSeleccionados) {
        int indexContenido = distrPred(eng);
        
        // Asegurarse de que no seleccionamos el índice del siguiente contenido
        if (!contenidoPredSelec[indexContenido] && indexContenido + 1 < numContenido) {
            contenidoPredSelec[indexContenido] = true;
            
            // Seleccionamos indexContenido como predecesor de indexContenido + 1
            ficheroSalida << "    (predecesor " << nombresContenido[indexContenido] << " " << nombresContenido[indexContenido + 1] << ")" << std::endl;
            ++predSeleccionados;
        }
    }
    ficheroSalida << endl;

    // Paralelos
    std::uniform_int_distribution<> distrPar(0, numContenido - 1);
    int numParalelos = distr(eng) / 2; // Número de pares de contenidos paralelos
    vector<pair<int, int>> parejasParalelas;

    while (parejasParalelas.size() < numParalelos) {
        int index1 = distrPar(eng);
        int index2 = distrPar(eng);
        if (index1 != index2) {
            auto pareja = make_pair(std::min(index1, index2), max(index1, index2));
            if (find(parejasParalelas.begin(), parejasParalelas.end(), pareja) == parejasParalelas.end()) {
                parejasParalelas.push_back(pareja);
                ficheroSalida << "    (paralelo " << nombresContenido[pareja.first] << " " << nombresContenido[pareja.second] << ")" << endl;
            }
        }
    }
    ficheroSalida << endl;

    // Selección aleatoria de series y películas que el usuario quiere ver
    std::uniform_int_distribution<> distrObj(2, numContenido - 1);
    int numObjSeleccionados = distrObj(eng); // Número de contenidos objetivo
	vector<bool> contenidoObjSelec (numContenido);

	int objseleccionados = 0;
	while (objseleccionados < numObjSeleccionados) {
		for (int i = 0; i < numObjSeleccionados; ++i) {
			if (numContenido > 0) {
				int indexcontenido = distrObj(eng) % numContenido;
				if (!contenidoObjSelec[indexcontenido]) {
					contenidoObjSelec[indexcontenido] = true;
					ficheroSalida << "    (quiere_ver " << nombresContenido[indexcontenido] << ")" << endl;
					++objseleccionados;
				}
			}
		}
	}
    ficheroSalida << endl;

    // Número de días
    for (int i = 0; i < nombresDias.size(); ++i) {
        ficheroSalida << "    (= (numero_dia " << nombresDias[i] << ") " << (i + 1) << ")" << std::endl;
    }
    ficheroSalida << endl;

    // Planificado para
    for (const auto& contenido : nombresContenido) {
        ficheroSalida << "    (= (planificado_para " << contenido << ") 0)" << std::endl;
    }
	ficheroSalida << endl;

	ficheroSalida << "  )" << endl << endl;

	ficheroSalida << "  (:goal (forall (?c - contenido) (or (not (quiere_ver ?c)) (visto ?c))))" << endl;
	ficheroSalida << ")";
}