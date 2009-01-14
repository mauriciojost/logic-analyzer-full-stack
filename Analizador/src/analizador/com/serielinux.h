/** \file 
 *  \brief Archivo usado para SO Linux.
 *
 *  Este archivo contiene la definición de las funciones de 
 *  de acceso al puerto, destinadas a SO tipo Linux
 *
 */

/** \defgroup HeaderLinux Funciones para Linux
 * @{
 */

#ifndef __SERIE_LINUX__
#define __SERIE_LINUX__

    #include <stdio.h>   /* Standard input/output definitions */
    #include <string.h>  /* String function definitions */
    #include <unistd.h>  /* UNIX standard function definitions */
    #include <fcntl.h>   /* File control definitions */
    #include <sys/ioctl.h>
    
    #include <termios.h> /* POSIX terminal control definitions */

    #ifndef FALSE
       #define FALSE 0
    #endif
    
    #ifndef TRUE
       #define TRUE  1
    #endif
    
    #ifndef BOOL
       #define BOOL  int
    #endif

    #define INVALID_HANDLE_VALUE -1
    #define NONE      0    
    #define RTSCTS    1
    #define HARD      1
    #define XONXOFF   2
    #define SOFT      2
    
    BOOL ERROR_CONFIGURE_PORT=FALSE;

    typedef  struct termios	DCB;
    typedef  int	        HANDLE;
    
/**
 * @}
 */

  /** \var HANDLE Open_Port(char COMx[])
   *  \brief Abre el puerto de comunicaciones
   *  \param COMx es el puerto a abrir "/dev/ttyS0","/dev/ttyS1","/dev/ttyACM0","/dev/ttyUSB0",...
   *  \return El manejador del puerto
   *  \ingroup HeaderLinux
   */
    HANDLE Open_Port(char COMx[])
    {
	int fd; // File descriptor for the port

	fd = open(COMx, O_RDWR | O_NOCTTY );//| O_NDELAY);

	if (fd <0)	
	{
		printf("open_port:fd=%d: No se puede abrir %s\n",fd,COMx);
		return INVALID_HANDLE_VALUE;
	}
	printf("open_port:fd=%d: Abierto puerto %s\n",fd,COMx);
	tcflush(fd, TCIOFLUSH);
	return (fd);
    }

    
  /** \var DCB Get_Configure_Port(HANDLE fd)
   *  \brief Devuelve la configuración Actual del Puerto serie 
   *  \param fd Es el manejador del puerto
   *  \return Una estructura con DCB con la configuración actual del puerto
   *  \ingroup HeaderLinux
   */  
    DCB Get_Configure_Port(HANDLE fd)
    {
	struct termios oldtio;
	if(tcgetattr(fd,&oldtio)!=0)  /* almacenamos la configuración actual del puerto */
	{
		printf("Error Pidiendo configuración de Puerto\n");
		ERROR_CONFIGURE_PORT=TRUE;
		return oldtio;
	}
	ERROR_CONFIGURE_PORT=FALSE;
	return oldtio;
    }
    
    
  /** \var DCB Configure_Port(HANDLE fd,unsigned int BaudRate,char CharParity[])
   *  \brief Configura el puerto serie 
   *  \param fd Es el manejador del puerto 
   *  \param BaudRate Es la velocidad del puerto
   *  \param CharParity indica el numero de bits de la transmisión "8N1","7E1","7O1","7S1" 
   *  \return Una estructura con Bcd con la configuración del puerto
   *  \ingroup HeaderLinux
   */    
    DCB Configure_Port(HANDLE fd,unsigned int BaudRate,char CharParity[])
    {
	DCB newtio;
	bzero(&newtio, sizeof(newtio));    //limpiamos struct para recibir los
                                           //nuevos parámetros del puerto
	//tcflush(fd, TCIOFLUSH);

	//CLOCAL  : conexion local, sin control de modem
	//CREAD   : activa recepcion de caracteres
	newtio.c_cflag =CLOCAL | CREAD ;
	
	cfsetispeed(&newtio,BaudRate);
	cfsetospeed(&newtio,BaudRate);
	
	if(strncmp(CharParity,"8N1",3)==0)	//CS8     : 8n1 (8bit,no paridad,1 bit de parada)
	{
		newtio.c_cflag &= ~PARENB;
		newtio.c_cflag &= ~CSTOPB;
		newtio.c_cflag &= ~CSIZE;
		newtio.c_cflag |= CS8;
	}
	if(strncmp(CharParity,"8E1",3)==0)
	{
		newtio.c_cflag |=PARENB;
		newtio.c_cflag &= ~PARODD;
		newtio.c_cflag &= ~CSTOPB;
		newtio.c_cflag &= ~CSIZE;
		newtio.c_cflag |= CS8;
	}
	if(strncmp(CharParity,"7E1",3)==0)
	{
		newtio.c_cflag |= PARENB;
		newtio.c_cflag &= ~PARODD;
		newtio.c_cflag &= ~CSTOPB;
		newtio.c_cflag &= ~CSIZE;
		newtio.c_cflag |= CS7;
	}
	if(strncmp(CharParity,"7O1",3)==0)
	{
		newtio.c_cflag |= PARENB;
		newtio.c_cflag |= PARODD;
		newtio.c_cflag &= ~CSTOPB;
		newtio.c_cflag &= ~CSIZE;
		newtio.c_cflag |= CS7;
	}
	if(strncmp(CharParity,"7S1",3)==0)
	{
		newtio.c_cflag &= ~PARENB;
		newtio.c_cflag &= ~CSTOPB;
		newtio.c_cflag &= ~CSIZE;
		newtio.c_cflag |= CS8;
	}

	// IGNPAR  : ignora los bytes con error de paridad
	// ICRNL   : mapea CR a NL (en otro caso una entrada CR del otro ordenador
	// no terminaría la entrada) en otro caso hace un dispositivo en bruto
	// (sin otro proceso de entrada)
	newtio.c_iflag = 0;
	//newtio.c_iflag = IGNPAR;
	//newtio.c_iflag |= ICRNL;

	//Salida en bruto.
	newtio.c_oflag = 0;

	//ICANON  : activa entrada canónica(Modo texto)
	//desactiva todas las funcionalidades del eco, y no envía señales al
	//programa llamador
	//newtio.c_lflag = ICANON;
	newtio.c_lflag = 0;

	// inicializa todos los caracteres de control
	// los valores por defecto se pueden encontrar en /usr/include/termios.h,
	// y vienen dadas en los comentarios, pero no los necesitamos aqui

	newtio.c_cc[VTIME]    = 0;     /* temporizador entre caracter, no usado */
	newtio.c_cc[VMIN]     = 1;     /* bloquea lectura hasta llegada de un caracter */
	
	if(tcsetattr(fd,TCSANOW,&newtio)!=0)
	{
		printf("ERROR: No se pudo poner Configuración del Puerto\n" );
		ERROR_CONFIGURE_PORT=TRUE;
		return newtio;
	}

	return newtio;
    }
    
    
  /** \var int Set_Configure_Port(HANDLE fd,DCB PortDCB)
   *  \brief Coloca la configuración en el puerto serie
   *  \param fd Es el manejador del puerto 
   *  \param PortDCB es la configuración del puerto
   *  \return TRUE si todo fue bien o FALSE si no 
   *  \ingroup HeaderLinux
   */  
    int Set_Configure_Port(HANDLE fd,DCB newtio)
    {
	// ahora limpiamos el buffer de entrada y salida del modem y activamos 
	// la configuración del puerto
	//tcflush(fd, TCIOFLUSH);
	
	if(tcsetattr(fd,TCSANOW,&newtio)!=0)
	{
		printf("ERROR: No se pudo poner Configuración del Puerto\n" );
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
   *  \ingroup HeaderLinux
   */
  
/*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                      FUNCION A REEMPLAZAR */
    long Write_Port(HANDLE fd,char Data[],int SizeData)
    {
	//return  write(fd,Data,SizeData);
  return _write_asm(fd,Data);
    }
    
    
  /** \var long Read_Port(HANDLE fd,char *Data,int SizeData)
   *  \brief Recibe datos en el puerto serie
   *  \param fd Es el manejador del puerto 
   *  \param Data Es el arreglo donde se almacenarán los datos recibidos
   *  \param SizeData es el tamaño del arreglo
   *  \return En caso de éxito, se devuelve el número de bytes recibidos (cero 
   *          indica que no se ha recibido nada). En caso de error, se devuelve -1 
   *  \ingroup HeaderLinux
   */  
    long Read_Port(HANDLE fd,char *Data,int SizeData)
    {
	struct termios newtio;
	struct timeval inic,fin,temp;
	float tiempo,t;
	int bytes;
	int ESTADO;
	int TEMPO;

	if(tcgetattr(fd,&newtio)!=0) return -1;
	else
	{
		ESTADO=newtio.c_cc[VMIN];
		TEMPO=newtio.c_cc[VTIME];
		if( (ESTADO==0)&&(TEMPO==0) )
		{
			return read(fd,Data,SizeData);
		}
		else
		{
			if(TEMPO==0)
			{
				do{ioctl(fd, FIONREAD, &bytes);}
				while(bytes<SizeData);
				return read(fd,Data,SizeData);
			}
			else
			{
				gettimeofday(&inic,NULL);
				tiempo=newtio.c_cc[VTIME];
				do
				{
					gettimeofday(&fin,NULL);
					temp.tv_sec=fin.tv_sec-inic.tv_sec;
					temp.tv_usec=fin.tv_usec-inic.tv_usec ;
					t=((temp.tv_usec/1000.0)+temp.tv_sec*1000.0)/100.0;
				}while( (t<tiempo*SizeData) && (Kbhit_Port(fd)<SizeData) );

				if(Kbhit_Port(fd)!=0)	return read(fd,Data,SizeData);
				else			return 0;
			}
		}
	}
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
   *  \ingroup HeaderLinux    
   */
    long Gets_Port(HANDLE fd,char *Data,int SizeData)
    {
	struct termios newtio;
	long n=0,i=0;
	struct timeval inic,fin,temp;
	float tiempo,t;

	if(tcgetattr(fd,&newtio)!=0) return -1;

	for(i=0;i<SizeData;i++)
	{
		gettimeofday(&inic,NULL);
		tiempo=(float)newtio.c_cc[VTIME];
		do
		{
			gettimeofday(&fin,NULL);
			temp.tv_sec=fin.tv_sec-inic.tv_sec;
			temp.tv_usec=fin.tv_usec-inic.tv_usec ;
			t=((temp.tv_usec/1000.0)+temp.tv_sec*1000.0)/100.0;
		}while( (t<tiempo) && (Kbhit_Port(fd)==0) );

		Data[i]=0;
		if( (Kbhit_Port(fd)!=0)||(tiempo==0) ) read(fd,&Data[i],1);

		if( ((Data[i]==13)||(Data[i]==10)||(Data[i]==0))&&(i!=0) )
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
   *  \ingroup HeaderLinux    
   */    
  /*XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX                      FUNCION A REEMPLAZAR */
    long Getc_Port(HANDLE fd,char *Data)
    {
	struct termios newtio;
	struct timeval inic,fin,temp;
	float tiempo,t;

	if(tcgetattr(fd,&newtio)!=0) return -1;

	gettimeofday(&inic,NULL);
	tiempo=(float)newtio.c_cc[VTIME];
	do
	{
		gettimeofday(&fin,NULL);
		temp.tv_sec=fin.tv_sec-inic.tv_sec;
		temp.tv_usec=fin.tv_usec-inic.tv_usec ;
		t=((temp.tv_usec/1000.0)+temp.tv_sec*1000.0)/100.0;
	}while( (t<tiempo) && (Kbhit_Port(fd)==0) );

	if( (Kbhit_Port(fd)!=0)||(tiempo==0) )	return read(fd,Data,1);
	else					return 0;
    }


  /** \var int Kbhit_Port(HANDLE fd)
   *  \brief Indica el numero de caracteres disponible en el buffer de entrada.
   *  \param fd Es el manejador del puerto. 
   *  \return El numero de caracteres en el buffer de recepción.
   *  \ingroup HeaderLinux    
   */   
    int Kbhit_Port(HANDLE fd)
    {
       int bytes;
       ioctl(fd, FIONREAD, &bytes);
       return bytes;

    }
    

  /** \var int Close_Port(HANDLE hComm)
   *  \brief Cierra el puerto serie.
   *  \param fd Es el manejador del puerto. 
   *  \return TRUE si se ha cerrado el Puerto y FALSE en el caso contrario.
   *  \ingroup HeaderLinux    
   */    
    int Close_Port(HANDLE fd)
    {
	 if (fd != INVALID_HANDLE_VALUE)
         {  // Close the communication port.
		// ahora limpiamos el buffer de entrada y salida del modem y activamos 
		// la configuración del puerto
		//tcflush(fd, TCIOFLUSH);
            if (close(fd)!=0)
            {printf("Error cerrando archivo\n");return FALSE;}
            else
            {fd = INVALID_HANDLE_VALUE;return TRUE;}
         }
	 return FALSE;
    }


  /** \var int Set_Hands_Haking(HANDLE fd,int FlowControl)
   *  \brief Configura el control de flujo en el puerto serie
   *  \param fd Es el manejador del puerto. 
   *  \param FlowControl 
   *                     0    Ninguno<br>                                            
   *                     1    RTS/CTS<br>                                             
   *                     2    Xon/Xoff<br>                                            
   *                     3    DTR/DSR  
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *  \ingroup HeaderLinux    
   */    
    int Set_Hands_Haking(HANDLE fd,int FlowControl)
    {
	struct termios newtio;
	tcgetattr(fd,&newtio);  /* almacenamos la configuración actual del puerto */
	switch (FlowControl)
	{
		case 0://NONE
		{
			newtio.c_cflag &= ~CRTSCTS;
			newtio.c_iflag &= ~(IXON | IXOFF | IXANY);
			newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
			newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
			break;
		}
		case 1://RTS/CTS - HARD
		{
			newtio.c_cflag |= CRTSCTS;
			newtio.c_iflag &= ~(IXON | IXOFF | IXANY);
			newtio.c_cc[VSTART]   = 0;     /* Ctrl-q */
			newtio.c_cc[VSTOP]    = 0;     /* Ctrl-s */
			break;
		}
		case 2://XON/XOFF - SOFT
		{
			newtio.c_cflag &= ~CRTSCTS;
			newtio.c_iflag |= (IXON | IXOFF );//| IXANY);
			newtio.c_cc[VSTART]   = 17;     /* Ctrl-q */
			newtio.c_cc[VSTOP]    = 19;     /* Ctrl-s */
			break;
		}
	}
	tcsetattr(fd, TCSANOW, &newtio);
	return 0;
    }


  /** \var int Set_RThreshold(HANDLE fd,int n)
   *  \brief configura el numero mínimo de caracteres que permitirá
   *         que se ejecute la lectura del puerto
   *  \param fd Es el manejador del puerto. 
   *  \param n  es el numero de caracteres que activara la lectura
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *  \ingroup HeaderLinux    
   */        
    int Set_RThreshold(HANDLE fd,int n)
    {
	    return TRUE;
    }
    
  /** \var int Set_BaudRate(HANDLE fd,unsigned int BaudRate)
   *  \brief configura la  velocidad puerto serie
   *  \param fd Es el manejador del puerto. 
   *  \param BaudRate Es la velocidad del Puerto
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *  \ingroup HeaderLinux    
   */    
    int Set_BaudRate(HANDLE fd,unsigned int BaudRate)
    {
	struct termios newtio;

	if(tcgetattr(fd,&newtio)!=0)
        {
              printf("Error obteniendo configuración time-out actual\n");
              return FALSE;
        }
	
	cfsetispeed(&newtio, BaudRate);
	cfsetospeed(&newtio, BaudRate);
	
	if(tcsetattr(fd, TCSANOW, &newtio)!=0)
        {
              printf("Error estableciendo nueva configuración time-out\n");
              return FALSE;
        }
	
	return TRUE;	
    }
	

  /** \var int Set_Time(HANDLE fd,unsigned int Time)
   *  \brief configura Temporizador para read y write
   *  \param fd Es el manejador del puerto. 
   *  \param Time Tiempo entre bits, T=Time*0.1s, para tamaño total de time-out   
   *               en read y write.<br>
   *               Timeout = (Time *0.1* number_of_bytes) seg 
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *  \ingroup HeaderLinux    
   */  
    int Set_Time(HANDLE fd,unsigned int Time) //t =Time*0.1 s)
    {
	struct termios newtio;
        /* almacenamos la configuracion actual del puerto */
	if(tcgetattr(fd,&newtio)!=0)
        {
              printf("Error obteniendo configuración time-out actual\n");
              return FALSE;
        }
        
	newtio.c_cc[VTIME]    = Time;/*temporizador entre caracter*/
	newtio.c_cc[VMIN]     = 1;   /*bloquea lectura hasta llegada de un
                                       caracter  */
	
	if(tcsetattr(fd, TCSANOW, &newtio)!=0)
        {
              printf("Error estableciendo nueva configuración time-out\n");
              return FALSE;
        }
	
	    return TRUE;	
    }
    
  /** \var int IO_Blocking(HANDLE fd,int Modo)
   *  \brief configura Temporizador para read y write
   *  \param fd Es el manejador del puerto. 
   *  \param Modo<br>
   *   TRUE : Modo bloqueante<br>                                          
   *   FALSE: Modo no bloqueante 
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *  \ingroup HeaderLinux    
   */  
    int IO_Blocking(HANDLE fd,int Modo) 
    {
	struct termios newtio;
        /* almacenamos la configuracion actual del puerto */
	if(tcgetattr(fd,&newtio)!=0)
        {
		printf("Error obteniendo configuración time-out actual\n");
		return FALSE;
        }
	
	if(Modo==FALSE)
	{
		newtio.c_cc[VTIME]    = 0;     /* temporizador entre caracter*/
		newtio.c_cc[VMIN]     = 0;     /* bloquea lectura hasta llegada de caracter. 1 */
	}
	if(Modo==TRUE)
	{
		newtio.c_cc[VTIME]    = 0;     /* temporizador entre caracter*/
		newtio.c_cc[VMIN]     = 1;     /* bloquea lectura hasta llegada de caracter. 1 */
	}
	
	if(tcsetattr(fd, TCSANOW, &newtio)!=0)
        {
		printf("Error estableciendo nueva configuración time-out\n");
		return FALSE;
        }
	
	return TRUE;	
    }
  /** \var int Clean_Buffer(HANDLE fd)
   *  \brief Termina las operaciones de lectura y escritura pendientes y limpia
   *         las colas de recepción y de transmisión.
   *  \param fd Es el manejador del puerto. 
   *  \return TRUE si todo fue bien y FALSE si no lo fue.
   *  \ingroup HeaderLinux    
   */  
    int Clean_Buffer(HANDLE fd) 
    {
       	if(tcflush(fd, TCIOFLUSH)!=0)
        {
              printf("Error Limpiando el Buffer  de entrada y salida\n");
              return FALSE;
        }
       return TRUE;
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
       return TRUE;
    }
    
  /** \var HANDLE Create_Thread_Port(HANDLE *fd)
   *  \brief Se usa para crear un hilo que ejecuta la función <br>
   *         SERIAL_PORT_EVENT(HANDLE *hPort) <br>
   *         cuando se recibe un caracter por el puerto serie.
   *  \param fd Es el manejador del puerto. 
   *  \return El manejador del hilo creado 
   *  \ingroup HeaderLinux    
   */      
    #ifdef ENABLE_SERIAL_PORT_EVENT
    #include <pthread.h>
    void *Thread_Port(void *hPort)
    {
        int n=0;
        HANDLE *fd;
        fd=(HANDLE *)hPort;

        printf("SERIAL_PORT_EVENT [OK]\n");

        do {
            if(Kbhit_Port(*fd)!=0)
               SERIAL_PORT_EVENT(fd);
        } while(TRUE);
    }
    pthread_t Create_Thread_Port(HANDLE *fd) 
    {
         pthread_t idHilo;
         pthread_create (&idHilo, NULL, Thread_Port, fd);
         return idHilo;
    }
    #endif

#endif
