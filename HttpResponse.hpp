/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:48:43 by kpineda-          #+#    #+#             */
/*   Updated: 2026/02/20 19:26:23 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPRESPONSE_HPP
#define HTTPRESPONSE_HPP
#include <sstream>
#include <string>
#include <map>

class HttpResponse
{
	std::string version; // "HTTP/1.1"
	int statusCode;      // 200, 404, etc.
	std::string body;
	std::map<std::string, std::string> headers; // Map of header key-value pairs
	
	static std::map<int, std::string> statusMessages; // Map of status codes to messages

public:
	HttpResponse();

	static void initializeStatusMessages(); // Initialize the status code to message mapping
	//methods to set response components
	void setStatusCode(int code);
	void setBody(const std::string& body);
	void addHeader(const std::string& key, const std::string& value);

	//method to convert response to string format
	std::string toString() const;
};

#endif 
