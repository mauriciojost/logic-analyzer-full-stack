#ifndef __COMUNICACION_SERIAL__
#define __COMUNICACION_SERIAL__
/**
   \mainpage LnxCOMM :: Linux Communication
   <br>
   \author Fernando Pujaico Rivera
   \date Mayo-2007
   \version v1.03

   \par Leeme:
   LnxCOMM te proporciona acceso al puerto serie de una manera facil <br> <br>
   LnxCOMM esta capacitado para trabajar tanto en sistemas operativos tipo Windows como Linux
   ademas proporciona las formas mas utilizadas para la lectura de datos, como el tipo de 
   lectura Bloqueante, No-Bloqueante, TimeOut y mediante eventos. <br> <br>
   <ul>    
   	<li>
        Preguntas y sugerencias a: fernando.pujaico.rivera@gmail.com
   	</li>
   	<li>
   	puedes seguir viendo mas bibliotecas artículos  y demas en 
   	<A HREF="http://zsoluciones.com">http://ZSoluciones.com</A>
   	</li>
   </ul>

   <br> <br>
   @verbatim
                                      __,,,,_
                       _ __..-;''`--/'/ /.',-`-.
                   (`/' ` |  \ \ \\ / / / / .-'/`,_
                  /'`\ \   |  \ | \| // // / -.,/_,'-,
                 /<7' ;  \ \  | ; ||/ /| | \/    |`-/,/-.,_,/')
                /  _.-, `,-\,__|  _-| / \ \/|_/  |    '-/.;.\'
                `-`  f/ ;      / __/ \__ `/ |__/ |
                     `-'      |  -| =|\_  \  |-' |
                           __/   /_..-' `  ),'  //
                          ((__.-'((___..-'' \__.'

   @endverbatim
*/

/** \file 
 *  \brief Archivo que selecciona el código según SO.
 *
 *  Este archivo es el encargado de seleccionar que código incluir
 *  según el sistema operativo.<br>
 *  para esto usa las siguientes definiciones:<br>
 *
 *  #define __WINDOWS_COM__  para sistemas operativos tipo WINDOWS<br>
 *  #define __LINUX_COM__    para sistemas operativos tipo LINUX
 */

////////////////////////////////////////////////////////////////////////////////
// HANDLE Open_Port(char COMx[]) : Abre el puerto de comunicaciones.          //
//             COMx.- Es el puerto a abrir  "COM1","COM2","COM3","COM4", ...  //
//             "/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3", ...       //
//             "/dev/ttyUSB0","/dev/ttyUSB1","/dev/ttyUSB2",...               //
//             Devuelve: el manejador del puerto.                             //
//                                                                            //
// DCB Get_Configure_Port(HANDLE fd) : Devuelve la configuración Actual del   //
//             Puerto serie.                                                  //
//             fd.- Es el manejador del puerto.                               //
//             Devuelve: Una estructura con DCB con la configuración actual   //
//             del puerto.                                                    //
//                                                                            //
// DCB Configure_Port(HANDLE fd,unsigned int BaudRate,char CharParity[])      //
//             : Configura el puerto serie.                                   //
//             fd.- Es el manejador del puerto.                               //
//             BaudRate.- Es la velocidad del puerto.                         //
//             CharParity.- indica el numero de bits de la transmisión.       //
//             "8N1","7E1","7O1","7S1"                                        //
//             Devuelve: Una estructura DCB con la configuración del puerto   //
//                                                                            //
// int Set_Configure_Port(HANDLE fd,DCB PortDCB) : coloca la configuración en //
//             el puerto serie.                                               //
//             fd.- Es el manejador del puerto.                               //
//             newtio.- es la configuracion del puerto.                       //
//             Devuelve: Una estructura con Bcd con la configuración actual   //
//             del puerto.                                                    //
//                                                                            //
// long Write_Port(HANDLE fd,char Data[],int SizeData): Escribe en el puerto  //
//             serie.                                                         //
//             fd.- Es el manejador del puerto.                               //
//             Data.- Es el dato a mandar.                                    //
//             SizeData.- es el tamaño de la cadena.                          //
//             Devuelve: En caso de éxito, se devuelve el número de bytes     //
//             escritos (cero indica que no se ha escrito nada). En caso de   //
//             error, se devuelve -1.                                         //
//                                                                            //
// long Read_Port(HANDLE fd,char *Data,int SizeData) : Recibe en el puerto    //
//             serie.                                                         //
//             fd.- Es el manejador del puerto.                               //
//             Data.- Es el dato a mandar.                                    //
//             SizeData.- es el tamaño de la cadena.                          //
//             Devuelve: El numero de caracteres recibidos.                   //
//                                                                            //
// long Gets_Port(HANDLE fd,char *Data,int SizeData) : Recibe en el puerto    //
//             serie,lee hasta encontrar un 0x0A,0xD o hasta completar        //
//             SizeData caracteres.                                           //
//             fd.- Es el manejador del puerto                                //
//             Data.- Es el dato a mandar                                     //
//             SizeData.- es el tamaño de la cadena                           //
//             Devuelve: El numero de caracteres recibidos                    //
//                                                                            //
// long Getc_Port(HANDLE fd,char *Data) : Recibe un caracter en puerto serie  //
//             fd.- Es el manejador del puerto                                //
//             Data.- Es el dato a mandar                                     //
//             SizeData.- es el tamaño de la cadena                           //
//             Devuelve: El numero de caracteres recibidos                    //
//                                                                            //
// int Kbhit_Port(HANDLE fd) : Indica el numero de caracteres disponible en   //
//             el buffer de entrada.                                          //
//             fd.- Manejador del fichero                                     //
//             Devuelve: El numero de caracteres en el buffer de recepción    //
//                                                                            //
// int Close_Port(HANDLE hComm) : Cierra el puerto serie                      //
//             fd.- Es el manejador del puerto.                               //
//             Devuelve: TRUE si se h cerrado el Puerto y FALSE en el caso    //
//             contrario.                                                     //
//                                                                            //
// int Set_Hands_Haking(HANDLE fd,int FlowControl) : Configura el control de  //
//             flujo en el puerto serie.                                      //
//             fd.- Es el manejador del puerto.                               //
//             FlowControl.- 0    Ninguno   1    RTS/CTS                      //
//                           2    Xon/Xoff  3    DTR/DSR                      //
//             Devuelve: TRUE si todo fue bien y FALSE si no lo fue.          //
//                                                                            //
// int Set_BaudRate(HANDLE fd,unsigned int BaudRate): configura la  velocidad //
//             puerto serie.                                                  //
//             fd.- Es el manejador del puerto                                //
//             BaudRate.- Es la velocidad del Puerto                          //
//             Devuelve: TRUE si todo fue bien y FALSE si no lo fue.          //
//                                                                            //
// int Set_Time(HANDLE fd,unsigned int Time) : configura Temporizador para    //
//             read y write.                                                  //
//             fd.- Es el manejador del puerto.                               //
//             Time.- multiplicador en m-seg para tamaño total de time-out en//
//             read y write                                                   //
//             Timeout = (10*Time-m-seg * number_of_bytes)                    //
//             Devuelve: TRUE si todo fue bien y FALSE si no lo fue           //
//                                                                            //
// int IO_Blocking(HANDLE fd,int Modo) : escoge como modo bloqueante o no     //
//             bloqueante para lectura.                                       //
//             fd.- Es el manejador del puerto.                               //
//             Modo.- TRUE : Modo bloqueante.                                 //
//                    FALSE: Modo no bloqueante.                              //
//             Devuelve: TRUE si todo fue bien y FALSE si no lo fue.          //
//                                                                            //
// int Clean_Buffer(HANDLE fd) : Termina las operaciones de lectura y         //
//             escritura pendientes y limpia las colas de recepción y de      // 
//             transmisión.                                                   //
//             fd.- Es el manejador del puerto.                               //
//             Devuelve: TRUE si todo fue bien y FALSE si no lo fue.          //
//                                                                            //
// HANDLE Create_Thread_Port(HANDLE *fd) Se usa para crear un hilo que        //
//             ejecuta la funcion.                                            //
//             SERIAL_PORT_EVENT(HANDLE *hPort)                               //
//             cuando se recibe un caracter por el puerto serie.              //
//             fd Es el manejador del puerto.                                 //
//             Devuelve: El manejador del hilo creado.                        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#ifdef __WINDOWS_COM__
#include "seriewindows.h"
#endif

#ifdef __LINUX_COM__
#include "serielinux.h"
#endif

#endif
