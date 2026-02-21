/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:48:49 by kpineda-          #+#    #+#             */
/*   Updated: 2026/02/20 19:39:12 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

std::map<int, std::string> HttpResponse::statusMessages;

void HttpResponse::initializeStatusMessages()
{
	if ( !statusMessages.empty() )
		return;
	statusMessages[200] = "OK";
	statusMessages[404] = "Not Found";
	statusMessages[403] = "Forbidden";
	statusMessages[500] = "Internal Server Error";
}

HttpResponse::HttpResponse() : version("HTTP/1.1"), statusCode(200), body("")
{
	initializeStatusMessages();
}

void HttpResponse::setStatusCode(int code)
{
	statusCode = code;
}

void HttpResponse::setBody(const std::string& body)
{
	this->body = body;

	std::stringstream ss;
	ss << body.length();
	addHeader("Content-Length", ss.str());
}

void HttpResponse::addHeader(const std::string& key, const std::string& value)
{
	headers[key] = value;
}

std::string HttpResponse::toString() const
{
	std::stringstream response;

	std::string msg = "Unknown Status";
	
	if (statusMessages.find(statusCode) != statusMessages.end())
		msg = statusMessages.at(statusCode);

	response << version << " " << statusCode << " " << msg << "\r\n";
	
	std::map<std::string, std::string>::const_iterator it;
	for (it = headers.begin(); it != headers.end(); ++it)
		response << it->first << ": " << it->second << "\r\n";
	response << "\r\n" << body;
	return response.str();
}