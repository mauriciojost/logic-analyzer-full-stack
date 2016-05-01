/** \file
 *  \brief Archivo usado para SO Windows.
 *
 *  Este archivo contiene la definición de las funciones de 
 *  de acceso al puerto, destinadas a SO tipo Windows
 *
 */

/** \defgroup HeaderWindows Funciones para Windows
 * @{
 */

#ifndef __SERIE_WINDOWS__
#define __SERIE_WINDOWS__

    #include <stdio.h>   
    #include <string.h>  
    #include <windows.h>
    #include "baudios.h"
    
    #define MAX_SIZE_BUFFER 8192

    #define NONE      0    
    #define RTSCTS    1
    #define HARD      1
    #define XONXOFF   2
    #define SOFT      2
    
    BOOL ERROR_CONFIGURE_PORT=FALSE;

  /** \var HANDLE Open_Port(char COMx[])
   *  \brief Abre el puerto de comunicaciones
   *  \param COMx es el puerto a abrir  "COM1","COM2","COM3","COM4",...
   *  \return El manejador del puerto
   *  
   */
    HANDLE Open_Port(char COMx[])
    {
         HANDLE fd;
         fd = CreateFile( COMx,	          	   // pointer to name of the file 
	             GENERIC_READ | GENERIC_WRITE, // access (read-write) mode 
	             0,				   // share mode 
	             NULL,			   // pointer to security attributes 
	             OPEN_EXISTING,		   // how to create
	             0,				   // file attributes
	             NULL);			   // handle to file with attributes
                                                   // to copy 

         if (fd == INVALID_HANDLE_VALUE) 
         {
              printf("Error:No se puede abrir puerto: %s \n",COMx); 
              return INVALID_HANDLE_VALUE;
         } 
         return (fd);
    }

  /** \var DCB Get_Configure_Port(HANDLE fd)
   *  \brief Devuelve la configuración Actual del Puerto serie 
   *  \param fd Es el manejador del puerto
   *  \return Una estructuracon DCB con la configuración actual del puerto
   *  
   */    
    DCB Get_Configure_Port(HANDLE fd)
    {
         DCB PortDCB;
         PortDCB.DCBlength = sizeof (DCB);     
         
         if(!GetCommState (fd, &PortDCB))
         {
               printf("Error Pidiendo configuracion de Puerto\n");
               ERROR_CONFIGURE_PORT=TRUE; 
               return PortDCB;        
         }
         ERROR_CONFIGURE_PORT=FALSE;
         return PortDCB;
    }

  /** \var DCB Configure_Port(HANDLE fd,unsigned int BaudRate,char CharParity[])
   *  \brief Configura el puerto serie
   *  \param fd Es el manejador del puerto
   *  \param BaudRate Es la velocidad del puerto
   *  \param CharParity indica el numero de bits de la transmisión "8N1","7E1","7O1","7S1"
   *  \return Una estructura con Bcd con la configuración del puerto
   *  
   */      
    DCB Configure_Port(HANDLE fd,unsigned int BaudRate,char CharParity[])
    {
         DCB PortDCB;
         PortDCB.DCBlength = sizeof (DCB); 
         
         // Configuramos tama�o de buffer de escritura/lectura
         if(!SetupComm(fd, MAX_SIZE_BUFFER, MAX_SIZE_BUFFER)) 
         {
               printf("Error configurando buffer\n");
               ERROR_CONFIGURE_PORT=TRUE;
               return PortDCB;
         }

         if(!GetCommState (fd, &PortDCB))
         {
               printf("Error Pidiendo configuración de Puerto\n");
               ERROR_CONFIGURE_PORT=TRUE;
               return PortDCB;
         }

         // Change the DCB structure settings.
         PortDCB.BaudRate = BaudRate;             // Current baud 
         PortDCB.fBinary = TRUE;                  // Binary mode; no EOF check 
         //PortDCB.EofChar = 0;
         PortDCB.fErrorChar = FALSE;              // Disable error replacement.
         PortDCB.fNull = FALSE;                   // Disable null stripping. 
         PortDCB.fAbortOnError = FALSE;           // Do not abort reads/writes on error.
         PortDCB.fParity = FALSE;                 // Disable parity checking.
         
         PortDCB.fRtsControl = RTS_CONTROL_DISABLE; // RTS flow control   
         
         PortDCB.fDtrControl = DTR_CONTROL_DISABLE; // DTR flow control type        

         PortDCB.fOutxCtsFlow = FALSE;            // No CTS output flow control 
         PortDCB.fOutxDsrFlow = FALSE;            // No DSR output flow control

         PortDCB.fDsrSensitivity = FALSE;         // DSR sensitivity 

         PortDCB.fOutX = FALSE;                   // No XON/XOFF out flow control 
         PortDCB.fInX = FALSE;                    // No XON/XOFF in flow control 
         PortDCB.fTXContinueOnXoff = TRUE;        // XOFF continues Tx 
         
         if(strncmp(CharParity,"8N1",3)==0)
         {
         PortDCB.ByteSize = 8;                    // Number of bits/bytes, 4-8 
         PortDCB.Parity = NOPARITY;               // 0-4=no,odd,even,mark,space 
         PortDCB.StopBits = ONESTOPBIT;           // 0,1,2 = 1, 1.5, 2 
         }
         if(strncmp(CharParity,"7E1",3)==0)
         {
         PortDCB.ByteSize = 7;                    // Number of bits/bytes, 4-8 
         PortDCB.Parity = EVENPARITY;             // 0-4=no,odd,even,mark,space
         PortDCB.StopBits = ONESTOPBIT;           // 0,1,2 = 1, 1.5, 2 
         }
         if(strncmp(CharParity,"7O1",3)==0)
         {
         PortDCB.ByteSize = 7;                    // Number of bits/bytes, 4-8 
         PortDCB.Parity = ODDPARITY;              // 0-4=no,odd,even,mark,space 
         PortDCB.StopBits = ONESTOPBIT;           // 0,1,2 = 1, 1.5, 2 
         }
         if(strncmp(CharParity,"7S1",3)==0)
         {
         PortDCB.ByteSize = 7;                    // Number of bits/bytes, 4-8 
         PortDCB.Parity = SPACEPARITY;            // 0-4=no,odd,even,mark,space 
         PortDCB.StopBits = ONESTOPBIT;           // 0,1,2 = 1, 1.5, 2 
         }   
         
         if (!SetCommState (fd, &PortDCB))
         {  // Could not configure the serial port.
            printf("Error: configurando puerto\n");
            ERROR_CONFIGURE_PORT=TRUE;
            return PortDCB;
         }

         // Configure timeouts 
         COMMTIMEOUTS timeouts;
         // No timeouts 
         timeouts.ReadIntervalTimeout = 0;
         timeouts.ReadTotalTimeoutMultiplier = 0;
         timeouts.ReadTotalTimeoutConstant = 0;
         timeouts.WriteTotalTimeoutMultiplier = 0;
         timeouts.WriteTotalTimeoutConstant = 0;

         if (!SetCommTimeouts(fd, &timeouts)) 
         {
	        printf("ERROR: No se pudo poner Comm Port timeouts: %s\n", 
                                                               GetLastError());
            ERROR_CONFIGURE_PORT=TRUE;
            return PortDCB;
         }


         ERROR_CONFIGURE_PORT=FALSE;
	     return PortDCB;
    }


  /** \var int Set_Configure_Port(HANDLE fd,DCB PortDCB)
   *  \brief Coloca la configuración en el puerto serie
   *  \param fd Es el manejador del puerto 
   *  \param PortDCB es la configuración del puerto
   *  \return TRUE si todo fue bien o FALSE si no 
   *  
   */  
    int Set_Configure_Port(HANDLE fd,DCB PortDCB)
    {
	 // ahora limpiamos el buffer de entrada y salida del modem
         // y activamos la configuración del puerto
         if (!SetCommState (fd, &PortDCB))
         {
            printf("Error: configurando puerto\n");
            ERROR_CONFIGURE_PORT=TRUE;
            return FALSE;
         }
         ERROR_CONFIGURE_PORT=FALSE;

	     return TRUE;
    }    

  /** \var long Write_Port(HANDLE fd,char Data[],int SizeData)
   *  \brief Escribe en el puerto serie
   *  \param fd Es el manejador del puerto 
   *  \param Data Es el dato a mandar
   *  \param SizeData es el tamaño del dato
   *  \return En caso de éxito, se devuelve el número de bytes escritos (cero 
   *          indica que no se ha escrito nada). En caso de error, se devuelve -1 
   *  
   */      
    long Write_Port(HANDLE fd,char Data[],int SizeData)
    {
         unsigned long n;

         WriteFile(fd,                // Port handle
                   Data,              // Pointer to the data to write
                   SizeData,          // Number of bytes to write
                   &n,                // Pointer to the number of bytes written
                   NULL);             // Must be NULL for Windows CE
         return n;
    }

  /** \var long Read_Port(HANDLE fd,char *Data,int SizeData)
   *  \brief Recibe datos en el puerto serie
   *  \param fd Es el manejador del puerto
   *  \param Data Es el arreglo donde se almacenarán los datos recibidos
   *  \param SizeData es el tamaño del arreglo
   *  \return En caso de éxito, se devuelve el número de bytes recibidos (cero
   *          indica que no se ha recibido nada). En caso de error, se devuelve -1
   *
   */
    long Read_Port(HANDLE fd,char *Data,int SizeData)
    {
         unsigned long n;

         ReadFile (fd,                // Port handle
                   Data,              // Pointer to the data to write
                   SizeData,          // Number of bytes to write
                   &n,                // Pointer to the number of bytes read
                   NULL);             // Must be NULL for Windows CE
         return n;
    }


  /** \var long Gets_Port(HANDLE fd,char *Data,int SizeData)
   *  \brief Recibe datos en el puerto serie,lee hasta encontrar un 0x0A,0x0D
   *         (rellenando el siguiente byte con un cero - "solo si existe")
   *         o hasta completar SizeData caracteres.
   *  \param fd Es el manejador del puerto
   *  \param Data Es el arreglo donde se almacenarán los datos recibidos
   *  \param SizeData es el tamaño máximo del arreglo
   *  \return En caso de éxito, se devuelve el número de bytes recibidos (cero
   *          indica que no se ha recibido nada). En caso de error, se devuelve -1
   *
   */
    long Gets_Port(HANDLE fd,char *Data,int SizeData)
    {
         unsigned long n=0,i=0;

         for(i=0;i<SizeData;i++)
         {
               ReadFile(fd,&Data[i],1,&n,NULL);
               if(((Data[i]==13)||(Data[i]==10)||(Data[i]==0))&&(i!=0))
               {
                n=i+1;
                if(n<SizeData) Data[n]=0;
                i=SizeData;
               }
         }

         return n;
    }

  /** \var Getc_Port(HANDLE fd,char *Data)
   *  \brief Recibe un caracter en el puerto serie.
   *  \param fd Es el manejador del puerto
   *  \param Data Es el dato(8-bit) a mandar
   *  \return En caso de éxito, se devuelve el número de bytes recibidos (cero
   *          indica que no se ha recibido nada). En caso de error, se devuelve -1
   *
   */
    long Getc_Port(HANDLE fd,char *Data)
    {
         unsigned long n;

         ReadFile(fd,Data,1,&n,NULL);

         return n;
    }

  /** \var int Kbhit_Port(HANDLE fd)
   *  \brief Indica el numero de caracteres disponible en el buffer de entrada.
   *  \param fd Es el manejador del puerto.
   *  \return El numero de caracteres en el buffer de recepción.
   *
   */
    int Kbhit_Port(HANDLE fd)
    {
     	DWORD x;
     	COMSTAT cs;
     	// Actualizar COMSTAT, sirve para averiguar el número de bytes en el 
     	// buffer de entrada:
     	ClearCommError(fd, &x, &cs);
     	return cs.cbInQue;
    }


  /** \var int Close_Port(HANDLE hComm)
   *  \brief Cierra el puerto serie.
   *  \param fd Es el manejador del puerto. 
   *  \return TRUE si se ha cerrado el Puerto y FALSE en el caso contrario.
   *      
   */    
    int Close_Port(HANDLE hComm)
    {
         if (hComm != INVALID_HANDLE_VALUE)
         {  // Close the communication port.
            
            // Liberar máscara de eventos del puerto serie:
            SetCommMask(hComm, 0);
            
            if (!CloseHandle (hComm))
            {printf("Error cerrando archivo\n");return FALSE;}
            else
            {hComm = INVALID_HANDLE_VALUE;return TRUE;}
         }
	     return FALSE;
    }


  /** \var int Set_Hands_Haking(HANDLE fd,int FlowControl)
   *  \brief Configura el control de flujo en el puerto serie
   *  \param fd Es el manejador del puerto. 
   *  \param FlowControl 
   *                     0    Ninguno                                            
   *                     1    RTS/CTS                                             
   *                     2    Xon/Xoff                                            
   *                     3    DTR/DSR  
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *      
   */    
    int Set_Hands_Haking(HANDLE fd,int FlowControl)
    {
        DCB PortDCB;
        if(!GetCommState (fd, &PortDCB))
         {
               printf("Error Pidiendo configuración de Puerto\n");
               ERROR_CONFIGURE_PORT=TRUE;
               return FALSE;
         }
        
        switch(FlowControl)
        {
         case 0: ///NONE
              {
              PortDCB.fOutX = FALSE;                      // No XON/XOFF out flow control 
              PortDCB.fInX = FALSE;                       // No XON/XOFF in flow control 
              
              PortDCB.fRtsControl = RTS_CONTROL_ENABLE;   // RTS flow control  
               
              PortDCB.fDtrControl = DTR_CONTROL_ENABLE;   // DTR flow control type
              break;
              }
         case 1: ///RTS/CTS
              {
              PortDCB.fRtsControl = RTS_CONTROL_HANDSHAKE;// RTS flow control  
              break;
              }
         case 2: ///XON/OFF
              {
              PortDCB.fOutX = TRUE;                       // XON/XOFF out flow control 
              PortDCB.fInX = TRUE;                        // XON/XOFF in flow control  
              PortDCB.XonChar=0x11;                       // ASCII_XON
              PortDCB.XoffChar=0x13;                      // ASCII_XOFF
              PortDCB.XonLim=100;
              PortDCB.XoffLim=100;
              break;
              }
         case 3: ///DTR/DSR
              {
              PortDCB.fDtrControl = DTR_CONTROL_HANDSHAKE;// DTR flow control type
              break;
              }
        }
        
        if (!SetCommState (fd, &PortDCB))
        {
              printf("Error: configurando puerto\n");
              ERROR_CONFIGURE_PORT=TRUE;
              return FALSE;
        }
        ERROR_CONFIGURE_PORT=FALSE;

	    return TRUE;
    }
    

  /** \var int Set_RThreshold(HANDLE fd,int n)
   *  \brief configura el numero mínimo de caracteres que permitirá
   *         que se ejecute la lectura del puerto
   *  \param fd Es el manejador del puerto. 
   *  \param n  es el numero de caracteres que activara la lectura
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *      
   */        
    int Set_RThreshold(HANDLE fd,int n)
    {
	    return TRUE;
    }
    
  /** \var int Set_BaudRate(HANDLE fd,unsigned int BaudRate)
   *  \brief configura la  velocidad puerto serie
   *  \param fd Es el manejador del puerto. 
   *  \param BaudRate Es la velocidad del PUerto
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *      
   */     
    int Set_BaudRate(HANDLE fd,unsigned int BaudRate)
    {
        DCB PortDCB;

        if(!GetCommState (fd, &PortDCB))
         {
               printf("Error Pidiendo configuracion de Puerto\n");
               ERROR_CONFIGURE_PORT=TRUE;
               return FALSE;
         }
         
        PortDCB.BaudRate = BaudRate;                // Binary mode; no EOF check 

        if (!SetCommState (fd, &PortDCB))
        {
              printf("Error: configurando puerto\n");
              ERROR_CONFIGURE_PORT=TRUE;
              return FALSE;
        }
        ERROR_CONFIGURE_PORT=FALSE;

	    return TRUE;
    }

  /** \var int Set_Time(HANDLE fd,unsigned int Time)
   *  \brief configura Temporizador para read y write
   *  \param fd Es el manejador del puerto. 
   *  \param Time Tiempo entre bits, T=Time*0.2s, para tamaño total de time-out   
   *               en read y write.
   *               Timeout = (Time *0.1* number_of_bytes) seg 
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *      
   */  
    int Set_Time(HANDLE fd,unsigned int Time) 
    {
        COMMTIMEOUTS CommTimeouts;

        if(!GetCommTimeouts (fd, &CommTimeouts))
        {
              printf("Error obteniendo configuracion time-out actual: %s\n", 
                                                              GetLastError());
              return FALSE;
        }

        // Tiempo maximo en mseg. entre caracteres consecutivos
        CommTimeouts.ReadIntervalTimeout = Time*200;

        // Time-Out=TotalTimeoutMultiplier*number_of_bytes+TotalTimeoutConstant

        // Especifique el multiplicador de tiempo fuera de lectura con el miembro 
        // ReadTotalTimeoutMultiplier. En cada operación de lectura , este número 
        // se multiplica por el número de bytes que la lectura espera recibir .
        CommTimeouts.ReadTotalTimeoutMultiplier = Time*100;
        // Constante a sumar al time-out total de recepcion
        CommTimeouts.ReadTotalTimeoutConstant = 0;

        // Igual que lectura
        CommTimeouts.WriteTotalTimeoutMultiplier = Time*100;
        // Igual que lectura
        CommTimeouts.WriteTotalTimeoutConstant = 0;

        // Establecemos nuevos valores de time-out
        if(!SetCommTimeouts (fd, &CommTimeouts)) 
        {
              printf("Error estableciendo nueva configuracion time-out: %s\n", 
                                                              GetLastError());
              return FALSE;
        }

        return TRUE;
    }

  /** \var int IO_Blocking(HANDLE fd,int Modo)
   *  \brief configura Temporizador para read y write
   *  \param fd Es el manejador del puerto. 
   *  \param Modo
   *   TRUE : Modo bloqueante<br>
   *   FALSE: Modo no bloqueante 
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *      
   */  
    int IO_Blocking(HANDLE fd,int Modo) 
    {
        COMMTIMEOUTS CommTimeouts;

        if(!GetCommTimeouts (fd, &CommTimeouts))
        {
              printf("Error obteniendo configuracion time-out actual: %s\n", 
                                                              GetLastError());
              return FALSE;
        }
        
        // Especifica que la operación de lectura debe regresar inmediatamente 
        // con los caracteres que ya se hayan recibido, incluso aunque no se 
        // haya recibido ninguno.
        if(Modo==FALSE)// No bloqueante
        {
         CommTimeouts.ReadIntervalTimeout = MAXDWORD;
         CommTimeouts.ReadTotalTimeoutMultiplier = 0;
         CommTimeouts.ReadTotalTimeoutConstant = 0;
         CommTimeouts.WriteTotalTimeoutMultiplier = 0;
         CommTimeouts.WriteTotalTimeoutConstant = 0;
        }
        // indica que el tiempo total de time-out no se usa para operaciones de
        // lectura/escritura.
        if(Modo==TRUE)// Bloqueante
        {
         CommTimeouts.ReadIntervalTimeout = 0;
         CommTimeouts.ReadTotalTimeoutMultiplier = 0;
         CommTimeouts.ReadTotalTimeoutConstant = 0;
         CommTimeouts.WriteTotalTimeoutMultiplier = 0;
         CommTimeouts.WriteTotalTimeoutConstant = 0;
        }

        if(!SetCommTimeouts (fd, &CommTimeouts)) 
        {
              printf("Error estableciendo nueva configuración time-out: %s\n", 
                                                              GetLastError());
              return FALSE;
        }

        return TRUE;
    }

  /** \var int Clean_Buffer(HANDLE fd)
   *  \brief Termina las operaciones de lectura y escritura pendientes y limpia
   *         las colas de recepción y de transmisión.
   *  \param fd Es el manejador del puerto. 
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *      
   */  
    int Clean_Buffer(HANDLE fd) 
    {
       return PurgeComm( fd , PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR );
    }
    
  /** \var int Setup_Comm(HANDLE fd,unsigned long InQueue,unsigned long OutQueue) 
   *  \brief Especifica el tamaño en Bytes del buffer de entrada y salida
   *  \param fd Es el manejador del puerto. 
   *  \param InQueue Especifica el tamaño en Bytes del buffer de entrada, se 
   *                 Recomienda el uso de numero pares.
   *  \param OutQueue Especifica el tamaño en Bytes del buffer de salida, se 
   *                 Recomienda el uso de numero pares.
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *      
   */  
    int Setup_Buffer(HANDLE fd,unsigned long InQueue,unsigned long OutQueue) 
    {
       return SetupComm(fd,InQueue,OutQueue);
    }


  /** \var HANDLE Create_Thread_Port(HANDLE *fd)
   *  \brief Se usa para crear un hilo que ejecuta la función <br>
   *         SERIAL_PORT_EVENT(HANDLE *hPort) <br>
   *         cuando se recibe un caracter por el puerto serie.
   *  \param fd Es el manejador del puerto. 
   *  \return El manejador del hilo creado 
   *      
   */      
    #ifdef ENABLE_SERIAL_PORT_EVENT  
    typedef  HANDLE pthread_t;

    void Thread_Port(HANDLE *hPort)
    {
        int n=0;

        printf("SERIAL_PORT_EVENT [OK]\n");

        do {
            if(Kbhit_Port(*hPort)!=0)
               SERIAL_PORT_EVENT(hPort);
        } while(TRUE);
    }
    pthread_t Create_Thread_Port(HANDLE *fd) 
    {
         pthread_t hilo;
         hilo = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Thread_Port, fd, 0, NULL);
         return hilo;
    }
    #endif

#endif

/**
 * @}
 */
