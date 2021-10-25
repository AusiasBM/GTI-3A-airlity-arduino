
// -*- mode: c++ -*-

// ----------------------------------------------------------
// PuertoSerie.h
// @autor: Aitor Benítez Estruch
// @fecha: 2021/10/05
// 
// @Descripcion:
// La clase PuertoSerie sirve para inicializar la UART0 utilizada para la comunicación
// entre la placa y el ordenador, principalmente para mostrar en pantalla procesos
// y valores de mediciones.
// ----------------------------------------------------------

#ifndef PUERTO_SERIE_H_INCLUIDO
#define PUERTO_SERIE_H_INCLUIDO

// ----------------------------------------------------------
// ----------------------------------------------------------
class PuertoSerie  {

public:
  // .........................................................
  // baudios:Z -> Constructor() ->
  //
  // El constructor inicia el Serial pasando el valor en baudios que queremos que funcione
  // 
  //@params baudios Valor en Baudios (normalmente 9600 o 115200)
  // .........................................................
  PuertoSerie (long baudios) {
	Serial.begin( baudios );
	// mejor no poner esto aquí: while ( !Serial ) delay(10);   
  } // ()

  // .........................................................
  // esperarDisponible()
  //
  // Este método bloquea cualquier proceso en la placa hasta que no se abra el puerto serie.
  // .........................................................
  void esperarDisponible() {

	while ( !Serial ) {
	  delay(10);   
	}

  } // ()

  // .........................................................
  // mensaje: T -> escribir() 
  //
  // escribir() imprime en pantalla cualquier tipo de información que se le pase
  //
  // @params mensaje Información que se quiere mostra en pantalla
  // .........................................................
  template<typename T>
  void escribir (T mensaje) {
	Serial.print( mensaje );
  } // ()
  
}; // class PuertoSerie

// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
// ----------------------------------------------------------
#endif
