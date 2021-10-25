// -*- mode: c++ -*-

// ----------------------------------------------------------
// Caracteristica.h
// @autor: Aitor Benítez Estruch
// @fecha: 2021/10/05
// 
// @Descripcion:
// 
// ----------------------------------------------------------

#ifndef CARACTERISTICA_H_INCLUIDO
#define CARACTERISTICA_H_INCLUIDO


// ----------------------------------------------------
//
// pString: Texto,
// pUint: Texto,
// tamMax: N -> stringAUint8AlReves()
// Texto <-
//
// ----------------------------------------------------
uint8_t * stringAUint8AlReves( const char * pString, uint8_t * pUint, int tamMax ) {

  int longitudString =  strlen( pString );
  int longitudCopiar = ( longitudString > tamMax ? tamMax : longitudString );
  // copio nombreServicio -> uuidServicio pero al revés
  for( int i=0; i<=longitudCopiar-1; i++ ) {
    pUint[ tamMax-i-1 ] = pString[ i ];
  } // for

  return pUint;
} // ()



class Caracteristica {
  private:
  uint8_t uuidCaracteristica[16] = { // el uuid se copia aquí (al revés) a partir de un string-c
    // least signficant byte, el primero
    '0', '1', '2', '3', 
    '4', '5', '6', '7', 
    '8', '9', 'A', 'B', 
    'C', 'D', 'E', 'F'
  };

  // 
  // 
  // 
  BLECharacteristic laCaracteristica;

  public:

  // .........................................................
  // .........................................................
  using CallbackCaracteristicaEscrita = void ( uint16_t conn_handle,
                         BLECharacteristic * chr,
                         uint8_t * data, uint16_t len); 
  // .........................................................
  // .........................................................

  // .........................................................
  /*
   * nombreCaracteristica:Text -> Constructor() ->
   */
  // .........................................................
  Caracteristica( const char * nombreCaracteristica_ )
    :
    laCaracteristica( stringAUint8AlReves( nombreCaracteristica_, &uuidCaracteristica[0], 16 ) )
  {
    
  } // ()

  // .........................................................
  // nombreCaracteristica:Text,
  // props:N,
  // permisoRead:N,
  // permisoWrite:N,
  // tamaño:N -> Constructor() ->
  // .........................................................
  Caracteristica( const char * nombreCaracteristica_ ,
          uint8_t props,
          SecureMode_t permisoRead,
          SecureMode_t permisoWrite, 
          uint8_t tam ) 
    :
    Caracteristica( nombreCaracteristica_ ) // llamada al otro constructor
  {
    (*this).asignarPropiedadesPermisosYTamanyoDatos( props, permisoRead, permisoWrite, tam );
  } // ()

  private:
  // .........................................................
  // CHR_PROPS_WRITE , CHR_PROPS_READ ,  CHR_PROPS_NOTIFY 
  //
  // N -> asignarPropiedades()
  //
  // .........................................................
  void asignarPropiedades ( uint8_t props ) {
    // no puedo escribir AUN si el constructor llama a esto: Serial.println( " laCaracteristica.setProperties( props ); ");
    (*this).laCaracteristica.setProperties( props );
  } // ()

  // .........................................................
  // BleSecurityMode::SECMODE_OPEN  , BleSecurityMode::SECMODE_NO_ACCESS
  //
  // N, N -> asignarPermisos()
  //
  // .........................................................
  void asignarPermisos( SecureMode_t permisoRead, SecureMode_t permisoWrite ) {
    // no puedo escribir AUN si el constructor llama a esto: Serial.println( "laCaracteristica.setPermission( permisoRead, permisoWrite ); " );
    (*this).laCaracteristica.setPermission( permisoRead, permisoWrite );
  } // ()

  // .........................................................
  //
  // N -> asignarTamanyoDatos()
  //
  // .........................................................
  void asignarTamanyoDatos( uint8_t tam ) {
    // no puedo escribir AUN si el constructor llama a esto: Serial.print( " (*this).laCaracteristica.setFixedLen( tam = " );
    // no puedo escribir AUN si el constructor llama a esto: Serial.println( tam );
    // (*this).laCaracteristica.setFixedLen( tam );
    (*this).laCaracteristica.setMaxLen( tam );
  } // ()

  public:
  
  // .........................................................
  //
  // props:N,
  // permisoRead:N,
  // permisoWrite:N,
  // tamaño:N -> asignarPropiedadesPermisosYTamanyoDatos() ->
  //
  // .........................................................
  void asignarPropiedadesPermisosYTamanyoDatos( uint8_t props,
                         SecureMode_t permisoRead,
                         SecureMode_t permisoWrite, 
                         uint8_t tam ) {
    asignarPropiedades( props );
    asignarPermisos( permisoRead, permisoWrite );
    asignarTamanyoDatos( tam );
  } // ()
                         

  // .........................................................
  // str:Text -> escribirDatos() <-
  // r:N <-
  // .........................................................
  uint16_t escribirDatos( const char * str ) {
    // Serial.print( " return (*this).laCaracteristica.write( str  = " );
    // Serial.println( str );

    uint16_t r = (*this).laCaracteristica.write( str );

    // Serial.print( ">>>Escritos " ); Serial.print( r ); Serial.println( " bytes con write() " );

    return r;
  } // ()

  // .........................................................
  //
  // str:Text -> NotificarDatos() <-
  // r:N <-
  // 
  // .........................................................
  uint16_t notificarDatos( const char * str ) {
    
    uint16_t r = laCaracteristica.notify( &str[0] );

    return r;
  } //  ()

  // .........................................................
  // .........................................................
  void instalarCallbackCaracteristicaEscrita( CallbackCaracteristicaEscrita cb ) {
    (*this).laCaracteristica.setWriteCallback( cb );
  } // ()

  // .........................................................
  //
  //  activar() ->
  //
  // .........................................................
  void activar() {
    err_t error = (*this).laCaracteristica.begin();
    Globales::elPuerto.escribir(  " (*this).laCaracteristica.begin(); error = " );
    Globales::elPuerto.escribir(  error );
  } // ()

}; // class Caracteristica

#endif
