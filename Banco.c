#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int buscar_usuario(char *user){
    FILE *bd = fopen("bd.csv","r");
    if(bd == NULL){
        printf("Error al abrir la base de datos");
    }
    //Establecemos que las lineas EJEMPLO DE LINEA -> (Manuel_Alonso,123445,1020.78) no van a superar los 100 caracteres
    char linea[100];
    //Creamos un Booleano para saber si se encontro o no el usuario
    int encontrado = 0;
    //Recorremos la Base de Datos
    while (fgets(linea, sizeof(linea), bd) != NULL){
        //La linea que leea va a pasar de esto (Manuel_Alonso,123445,1020.78) a esto (Manuel_Alonso)
        char *name = strtok(linea, ",");
        //Si 'name' es Igual a user lo marca como encontrado
        if (strcmp(name,user) == 0){
            encontrado = 1;
            //Salimos del bucle porque 'user' ya se encontro
            break;
        }
    }
    //Cerramos el archivo
    fclose(bd);
    //Retornamos 0 si el usuario no se encontro y 1 si se encontro
    return encontrado;
}

char* verificar_contraseña(const char *user) {
    //Creamos una variable Estatica para Almacenarla si se Encuentra
    static char contraseña[12];
    //Abrimos el Archivo que simula la Base de Datos
    FILE *bd = fopen("bd.csv", "r");
    if (bd == NULL) {
        printf("Error al abrir la base de datos\n");
        return NULL;
    }
    //Establecemos que las lineas EJEMPLO DE LINEA -> (Manuel_Alonso,123445,1020.78) no van a superar los 100 caracteres
    char linea[100];
    //Leemos el archivo linea por linea
    while (fgets(linea, sizeof(linea), bd) != NULL) {
        //Separamos la cadena y pasamos de esto (Manuel_Alonso,123445,1020.78) a esto (123445)
        char *name = strtok(linea, ",");
        char *pass = strtok(NULL, ",");
        //Verificamos que ni 'name' ni 'pass' sean NULL para que strcmp pueda comparar correctamente si 'name' y 'user' son iguales
        if (name && pass && strcmp(name, user) == 0) {
            //En caso de que lo sean en pass se va a guardar la contraseña de ese usuario y eliminara el salto de linea en caso de que exista
            pass[strcspn(pass, "\n")] = '\0';  
            //Con strcpy Copiamos la contraseña en pass
            strcpy(contraseña, pass);
            fclose(bd);
            return contraseña;
        }
    }
    fclose(bd);
    return NULL;
}

char* consultar_saldo(const char *user) {
    //Creamos una Variable Estatica para guadar el Saldo
    static char saldo[50];
    FILE *bd = fopen("bd.csv", "r");
    if (bd == NULL) {
        printf("Error al abrir la base de datos\n");
        return NULL;
    }
    //La lineas no van a superar los 100 caracteres
    char linea[100];
    //Recorremos el Archivo
    while (fgets(linea, sizeof(linea), bd) != NULL) {
        //Pasamos de esto (Manuel_Alonso,123445,1020.78) a esto (1020.78)
        char *name = strtok(linea, ",");
        char *pass = strtok(NULL, ",");
        char *saldo_str = strtok(NULL, ",");
        //Verificamos si 'name', 'pass' y 'saldo_str' no sean NULL, si no lo son comparamos el nombre de usuario almacenado con el que nos pasaron
        if (name && pass && saldo_str && strcmp(name, user) == 0) {
            //Eliminamos el salto de linea en caso de que exista
            saldo_str[strcspn(saldo_str, "\n")] = '\0';
            //Y copiamos el saldo en saldo_str
            strcpy(saldo, saldo_str);
            //Cerramos la Base de Datos porque el Saldo ya se encontro
            fclose(bd);
            return saldo;
        }
    }

    fclose(bd);
    //Si no encontramos el Saldo retornamos NULL
    return NULL;

}

void crear_usuario(){
    FILE *bd = fopen("bd.csv", "a");
    if (bd == NULL){
        printf("Ah ocurrido un error intentelo de nuevo mas tarde\n");
    }
    else{
    struct Datos
    {   char nombre[40];
        char contraseña[13];
        char saldo[10];
    };
    struct Datos usuario;
    char password[13];
    do{
        printf("Ingrese su nombre de usuario (Nombre/Apelldo): ");
        scanf("%s",usuario.nombre);
    } while (usuario.nombre[0] == '\0');
    do{
        printf("Ahora ingrese una contraseña: ");
        scanf("%s",password);
        printf("Verifique la contraseña: ");
        scanf("%s", usuario.contraseña);
        if (strcmp(usuario.contraseña, password) == 0){
            printf("Usuario registrado con exito\n");
        }
        else{
            printf("Error las contraseñas no coinciden\n");
        }
    }while(strcmp(usuario.contraseña, password) != 0);
    do{
        printf("Ahora ingrese su saldo: ");
        scanf("%s", usuario.saldo);
    }while(usuario.saldo[0] == '\0');
    fprintf(bd,"\n%s,%s,%s",usuario.nombre, usuario.contraseña, usuario.saldo);
    printf("Usuario creado con exito\n");
    
}
}

int main() {
    char user[100];
    char *password;
    int resultado;
    char contraseña[13];
    char *saldo;
    char respuesta[3];

    printf("Bienvenido ingrese su nombre: ");
    scanf("%s", user);

    resultado = buscar_usuario(user);

    if (resultado == 1) {
        do {
            printf("Bienvenido %s\n", user);
            password = verificar_contraseña(user);
            printf("Ingrese su contraseña: ");
            scanf("%s", contraseña);

            if (strcmp(contraseña, password) == 0) {
                saldo = consultar_saldo(user);
                printf("Su saldo es $%s\n", saldo);
            } else {
                printf("La contraseña es incorrecta\n");
            }
        } while (strcmp(contraseña, password) != 0);
    } else {
        printf("No se encontro el usuario %s\n", user);
        printf("Desea crear un nuevo usuario Si/No: ");
        scanf("%s", respuesta);
        if (strcmp(respuesta, "Si") == 0){
            crear_usuario();
        }
        else{
            printf("Saliendo del Banco\n");
        }
    }

    return 0;
}

