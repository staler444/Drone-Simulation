TRGDIR=./
OBJ=./obj
CPPFLAGS= -c -g -Wall -pedantic -std=c++17 -I inc

__start__: ${TRGDIR}dragonfly.out
	${TRGDIR}dragonfly.out

${OBJ}:
	mkdir ${OBJ}

${TRGDIR}dragonfly.out: ${OBJ} ${OBJ}/Dr3D_gnuplot_api.o ${OBJ}/dragonfly.o
	g++ -o ${TRGDIR}dragonfly.out ${OBJ}/dragonfly.o ${OBJ}/Dr3D_gnuplot_api.o -lpthread

${OBJ}/dragonfly.o: dragonfly.cpp inc/Dr3D_gnuplot_api.hh inc/drone.hh inc/figures.hh inc/macierz.hh inc/wektor.hh inc/przeszkoda.hh inc/scena.hh
	g++ ${CPPFLAGS} -o ${OBJ}/dragonfly.o dragonfly.cpp

${OBJ}/Dr3D_gnuplot_api.o: src/Dr3D_gnuplot_api.cpp inc/Dr3D_gnuplot_api.hh
	g++ ${CPPFLAGS} -o ${OBJ}/Dr3D_gnuplot_api.o src/Dr3D_gnuplot_api.cpp

inc/Dr3D_gnuplot_api.hh: inc/Draw3D_api_interface.hh
	touch inc/Dr3D_gnuplot_api.hh

clear:
	rm -rf ${OBJ} ${TRGDIR}dragonfly.out