/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:48:43 by kpineda-          #+#    #+#             */
/*   Updated: 2026/02/24 20:25:01 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP
#include <sstream>
#include <string>
#include <fstream>
#include <map>

class HttpResponse
{
	std::string version; // "HTTP/1.1"
	int statusCode;      // 200, 404, etc.
	std::string body;
	std::map<std::string, std::string> headers; // Map of header key-value pairs
	
	static std::map<int, std::string> statusMessages; // Map of status codes to messages
	static std::map<std::string, std::string> mimeTypes; // Map of file extensions to MIME types
	
	static void initializeMimeTypes(); // Initialize the MIME type mapping
public:
	HttpResponse();
	
	static void initializeStatusMessages(); // Initialize the status code to message mapping
	
	//methods to set response components
	void setStatusCode(int code);
	void setBody(const std::string& body);
	void addHeader(const std::string& key, const std::string& value);

	void loadFile(const std::string& path);

	//method to convert response to string format
	std::string toString() const;

	//method to reset response to default state
	void clear();
};

#endif 
