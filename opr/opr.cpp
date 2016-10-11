#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctime> 

struct slave
{
	char ip[20];
	int n;
};
int a = 1;
//пример работы без подачи аргументов автоматически
int main()
{
	int vibor;//переменна¤ отвечает за тип устройства(клиент - 1/ сервер - 0)
	scanf("%d", &vibor);
	//инициализаци¤ сокета
	char str[20];
	char tmp = ' ';
	int i = 0, j = 0, k = 0,
		m = 0, //–ј«ћ≈– ћј“–»÷џ
		N = 0, //  ќЋ»„≈—“¬ќ ’ќ—“ќ¬
		s, g = 0,
		h = 0,
		GLOBALM = 0,
		z = 0;

	FILE *f;
	/* —„»“ј≈“ –ј«ћ≈– ћј——»¬ј */
	//f=fopen(argv[1],"r");
	f = fopen("hosts.txt", "r");//открытие файла с хостами
	if (f == NULL)puts("error open file");
	//открыте файла дл¤ подсчета количества хостов
	for (N = 0; !feof(f); N++)
	{
		fscanf(f, "%s", str);
	}
	fclose(f);

	struct slave *HOSTS = (struct slave *)malloc(sizeof(struct slave)*(N));// инициализаци массива структур типа ’ќ—“ размерностью N

	/* «јѕќЋЌя≈“ ћј——»¬ ’ќ—“ќ¬ */
	//f=fopen(argv[1],"r");
	f = fopen("hosts.txt", "r");
	puts("Host list(0 - server/ 1..n - client):");
	for (i = 0; !feof(f); i++)
	{
		fscanf(f, "%s", &HOSTS[i].ip);
		printf("\tHOSTS[%d]: %s\n", i, HOSTS[i].ip);
	}
	fclose(f);

	// —„»“ј≈“  ќЋ»„≈—“¬ќ ЁЋ≈ћ≈Ќ“ќ¬
	//f=fopen(argv[3],"r");
	f = fopen("Matr.txt", "r");
	//подсчет количества строк в матрице
	while (tmp != '\n')
	{
		tmp = fgetc(f);
		if (tmp == ',')m++;//m отвечает за колчиество строк
	}
	fclose(f);
	//ќпределение числа строк и пор¤дка передачи между ѕк сети
	k = m / (N - 1); // ÷≈Ћќ≈ „»—Ћќ —“–ќ  ƒяЋ  ј∆ƒќ√ќ  Ћ»≈Ќ“ј
	j = m % (N - 1); // ќ—“ј“ќ  —“–ќ 
	for (i = 1; i<N; i++)//передача k строк какждому ѕк
		HOSTS[i].n = k;
	for (i = 1; i<(j + 1); i++)//ѕередча оставшихс¤ строк
		HOSTS[i].n++;
	/* „ј—“№ —≈–¬≈–ј */
	//if(atoi(argv[2])==0)
	if (vibor == 0)
	{
		int klm = 0;
		int *sockets = (int *)malloc(sizeof(int)*(N-1));//инициализаци¤ массива сокетов
		int err;
		struct sockaddr_in sin;
		s = socket(AF_INET, SOCK_STREAM, 0);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(1100);
		sin.sin_addr.s_addr = INADDR_ANY;
		bind(s, (struct sockaddr*)&sin, sizeof(sin));
		listen(s, N);

		i = 1;
		while (i<N)	//—в¤зь с ѕк сети
		{
			sockets[i] = accept(s, NULL, NULL);
			i++;
		}
		/* ѕќ—џЋј≈“ ’ќ—“јћ –ј«ћ≈– ћј“–»÷џ »  ќЋ»„≈—“¬ќ —“–ќ  */
		i = 1;
		while (i<N)
		{
			send(sockets[i], (char *)&m, sizeof(m), 0);
			recv(sockets[i], (char *)&m, sizeof(m), 0);
			send(sockets[i], (char *)&HOSTS[i].n, sizeof(HOSTS[i].n), 0);
			i++;
		}
		float **mas = (float **)malloc(sizeof(float)*(m));//инициализаци¤ двумерного массива дл¤ матрицы
		for (i = 0; i < m; i++)
			mas[i] =(float *)malloc(sizeof(float)*(m));
		float *massiv = (float *)malloc(sizeof(float)*(m));//массив дл¤ получени¤ строк от сервера
		f = fopen("Matr.txt", "r");
		if (f == NULL)puts("error open file");
		i = 0;
		while (i<m)
		{
			for (j = 1; (j<N) && (i<m); j++, i++)
			{
				/* ѕќЋ”„ј≈“ —“–ќ ” „»—≈Ћ */
				for (k = 0; k<m; k++)
				{
					fscanf(f, "%s", str);//меньше мароки чем с fgets(str,17,f)
					for (h = 0; h<17; h++)if (str[h] == ',')str[h] = '.';
					massiv[k] = atof(str);
				}


				z = sizeof(massiv); // –азмер получаемой строки
				send(sockets[j], (char *)&z, sizeof(z), 0); // ѕќ—Ћџј≈“ –ј«ћ≈– ћј——»¬ј
				send(sockets[j], (char *)&i, sizeof(i), 0); // Ќќћ≈– —“ –ќ »
				recv(sockets[j], (char *)&klm, sizeof (klm), 0);
				send(sockets[j], (char *)massiv, m*sizeof(float), 0); // —јћ” —“–ќ ”
				recv(sockets[j], (char *)&klm, sizeof (klm), 0);
			}
		}
		/*  ќЌ≈÷ ѕ≈–≈ƒј„» */
		z = -2;//пересылка строк завершена
		for (j = 1; j<N; j++)
			send(sockets[j], (char *)&z, sizeof(z), 0);
		fclose(f);
		h = -2;//клиент будет готовитьс¤ прин¤ть результирующую строк
		err = 2;//сигнал клиенту предоставить результирующую строку
		while (GLOBALM<m) // ѕ–ќ’ќƒ»ћ ћј“–»÷”
		{
			for (i = 1; (i<N) && (GLOBALM<m); i++, GLOBALM++)
			{
				// ѕроход по всем ѕк сети
				for (k = 1; k<N; k++)
				{
					if (k != i)
					{
						send(sockets[k], (char *)&h, sizeof(int), 0);
					}
				}
				int nras = 0;//номер разрешающей строки

				send(sockets[i], (char *)&err, sizeof(int), 0);
				recv(sockets[i], (char *)&nras, sizeof(int), 0);
				send(sockets[i], (char *)&klm, sizeof(int), 0);
				recv(sockets[i], (char *)massiv, m*sizeof(float), 0);
				for (j = 1; j<N; j++)
				{
					if (j != i)
					{
						//отправка данных о разрешающей строке+разрещающую строку
						//send(sockets[j], (char *)&h, sizeof(h), 0);
						send(sockets[j], (char *)&nras, sizeof(int), 0);
						recv(sockets[j], (char *)&klm, sizeof(int), 0);
						send(sockets[j], (char *)massiv, m*sizeof(float), 0);
					}
				}
			}
		}
		float p = 0;
		float oprel_buf = 0;//ранг, полученнный от одного хоста
		float opr = 1;//итоговый ранг матрицы
		f = fopen("out.txt", "w");//уточнить куда писать результат

		i = 0;
		j = 0;

		while (j < m) // ѕ–ќ’ќƒ»ћ ћј“–»÷”
		{
			for (i = 1; (i < N) && (j < m); i++, j++)
			{
				recv(sockets[i], (char *)massiv, m*sizeof(float), 0);
				send(sockets[i], (char *)&klm, sizeof(int), 0);
				//получение строк от клиентов,  хран¤щих итоговую матрицу
				int count = 0;
				//вывод итогов подсчетов в файл
				while (count < m)
				{
					if (massiv[count] == -0) massiv[count] = 0;
					fprintf(f, "%17.3f", massiv[count]);
					count++;
				}
				fprintf(f, "\n");
			}
		}

		i = 1;
		while (i < N)
		{
			recv(sockets[i], (char *)&oprel_buf, sizeof(float), 0);//получение ранга от клиента
			send(sockets[i], (char *)&a, sizeof(int), 0);
			opr *= oprel_buf;//суммирование значений временных рангов
			i++;
		}
		/*
		for (i = 1; i<N; i++)
		{
			recv(sockets[i], (char *)&oprel_buf, sizeof(float), 0);//получение ранга от клиента
			send(sockets[i], (char *)&a, sizeof(int), 0);
			opr *= oprel_buf;//суммирование значений временных рангов
			recv(sockets[i], (char *)&h, sizeof(int), 0);
			for (j = 0; j<h; j++)
			{
				recv(sockets[i], (char *)massiv, m*sizeof(float), 0);
				send(sockets[i], (char *)&klm, sizeof(int), 0);
				//получение строк от клиентов,  хран¤щих итоговую матрицу
				int count = 0;
				//вывод итогов подсчетов в файл
				while (count < m)
				{
					fprintf(f, "%17.3f", massiv[count]);
					count++;
				}
				fprintf(f, "\n");
			}
			printf("Output file is created\n");
		}*/
		fprintf(f, "ќпределитель матрицы равен %f", opr);
		fclose(f);
		//закрытие сокетов
		for (i = 1; i<N; i++)close(sockets[i]);
		/* -----------------------------------------------------*/
		for (i = 0; i < m; i++) //освобождение пам¤ти
			free (mas[i]);
		free (mas);
		printf("Server work is ended\nPress enter to continue...");
	}
	if (vibor == 1)
	{
		int M = 0, n = 0;//размерности массивов
		float mc = 0, ms = 0;
		s = socket(AF_INET, SOCK_STREAM, 0);
		struct sockaddr_in anAddr;
		anAddr.sin_family = AF_INET;
		anAddr.sin_port = htons(1100);
		anAddr.sin_addr.s_addr = inet_addr(HOSTS[0].ip);
		connect(s, (struct sockaddr *)&anAddr, sizeof(struct sockaddr));
		recv(s, (char *)&m, sizeof(m), 0);
		send(s, (char *)&m, sizeof(m), 0);
		recv(s, (char *)&M, sizeof(M), 0);
		float **massivc = (float **)malloc(sizeof(float)*(m));
		for (i = 0; i < M; i++)
			massivc[i] = (float *)malloc(sizeof(float)*(m));
		float *mastmp = (float *)malloc(sizeof(float)*(m));//массив дл¤ хранени¤ результирующей строки
		int *mas_m = (int *)malloc(sizeof(int)*(M));
		i = 0;
		
		int strn = 0;
		recv(s, (char *)&z, sizeof(z), 0);//определени¤ характера дальнейших действий клиента
		while (z>-1)
		{
			//получение необходимых данных
			recv(s, (char *)&mas_m[i], sizeof(mas_m[i]), 0);
			send(s, (char *)&a, sizeof(int), 0);
			recv(s, (char *)massivc[i], m*sizeof(float), 0);
			send(s, (char *)&a, sizeof(int), 0);
			recv(s, (char *)&z, sizeof(int), 0);
			strn++;
			i++;
		}
		int kr = 0;
		float mnoj = 1;//накопление произв разр ел
  		while (GLOBALM<m)//проход по всем строкам массива
		{
			int cn; // счетчики
			int cm;
			recv(s, (char *)&i, sizeof(int), 0);
			if (i>-1)
			{
				/* Mi part 
				double tempv = massivc[kr][mas_m[kr]];//получение разрещающего элемента
				if (tempv != 0)//проверка на валидность
				{
					cn = 0;
					while (cn < m)
					{
						massivc[kr][cn] /= tempv;
						cn++;
					}
				}*/
				send(s, (char *)&mas_m[kr], sizeof(mas_m[kr]), 0); // отправка на клиент є разрешающей строки
				recv(s, (char *)&a, sizeof(a), 0);
				float koef = 0;//коэффициент дл¤ нахождени¤ элементов матрицы
				cm = 0;
				koef = massivc[kr][mas_m[kr]];
				while (cm < m)
				{
					massivc[kr][cm] /= koef;//вычитание от элементов матрицы элементов разрещающей строки
					cm++;
				}
				mnoj *= koef;
				send(s, (char *)massivc[kr], m*sizeof(float), 0);//отправка на клиент разрещающей строки
				//recv(s, (char *)&a, sizeof(a), 0);
				cn = kr;
				cn++;
				while (cn < strn)
				{
					cm = 0;
					koef = massivc[cn][mas_m[kr]] / massivc[kr][mas_m[kr]];
					while (cm < m)
					{
						massivc[cn][cm] -= koef * massivc[kr][cm];//вычитание от элементов матрицы элементов разрещающей строки
						cm++;
					}
					cn++;
				}
				kr++;
			}
			else
			{
				//если клиент не содержит разрешающей строки
				int rasrn = 0;
				recv(s, (char *)&rasrn, sizeof(int), 0);
				send(s, (char *)&a, sizeof(int), 0);
				recv(s, (char *)mastmp, m*sizeof(float), 0);
				//send(s, (char *)&a, sizeof(a), 0);
				float koef = 0;
				cn = kr;
				//cn++;
				while (cn < strn)
				{
					koef = massivc[cn][rasrn] / mastmp[rasrn];
					cm = 0;
					while (cm < m)
					{
						massivc[cn][cm] -= mastmp[cm] * koef;
						cm++;
					}
					cn++;
				}
			}
			GLOBALM++;
		}
		int k = 0;
		float opr = 1;
		//локальное вычисление ранга матрицы на клиенте
		while (k < strn)
		{
			opr *= massivc[k][mas_m[k]];
			k++;
		}
		opr *= mnoj;
		/*send(s, (char *)&opr, sizeof(float), 0);//пересылка серверу данных о лакальном ранге
		recv(s, (char *)&a, sizeof(int), 0);
		send(s, (char *)&M, sizeof(int), 0);
		for (i = 0; i<M; i++)
		{
		//отправка резултатов вычислени¤ на клиенте серверу
		send(s, (char *)massivc[i], m*sizeof(float), 0);
		recv(s, (char *)&a, sizeof(int), 0);
		}*/

		i = 0;
		while (i < M)
		{
			send(s, (char *)massivc[i], m*sizeof(float), 0);
			recv(s, (char *)&a, sizeof(int), 0);
			i++;
		}

		send(s, (char *)&opr, sizeof(float), 0);//пересылка серверу данных о лакальном ранге
		recv(s, (char *)&a, sizeof(int), 0);
		for (int i = 0; i < M; i++)
			free(massivc[i]);
		free(massivc);
		close(s);//завершение работы сокетов

		printf("Client work is ended\nPress enter to continue...");
	}
	getchar();
	return 0;
}

