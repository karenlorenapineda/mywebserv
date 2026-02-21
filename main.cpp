/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 19:24:44 by kpineda-          #+#    #+#             */
/*   Updated: 2026/02/20 19:53:59 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"
#include <iostream>

int main() {
    // 1. Inicializamos los mensajes (esto lo harías una vez al arrancar el servidor)
    HttpResponse::initializeStatusMessages();

    std::cout << "--- PRUEBA 1: Respuesta 200 OK ---" << std::endl;
    HttpResponse res200;
    res200.setStatusCode(200);
    res200.setBody("<html><body><h1>Hola desde mi servidor!</h1></body></html>");
    res200.addHeader("Content-Type", "text/html");
    res200.addHeader("Server", "MiWebserver/1.0");
    
    std::cout << res200.toString() << std::endl;

    std::cout << "\n--- PRUEBA 2: Respuesta 404 Not Found ---" << std::endl;
    HttpResponse res404;
    res404.setStatusCode(404);
    res404.setBody("<html><body><h1>404 Pagina no encontrada</h1></body></html>");
    res404.addHeader("Content-Type", "text/html");
    
    std::cout << res404.toString() << std::endl;

	std::cout << "--- PRUEBA 3: Verificando Content-Length automático ---" << std::endl;
	HttpResponse testLen;
	testLen.setStatusCode(200);
	testLen.setBody("12345"); // Esto mide 5 bytes
	// No llamamos a addHeader("Content-Length", ...)

	std::cout << testLen.toString() << std::endl;

    return 0;
}