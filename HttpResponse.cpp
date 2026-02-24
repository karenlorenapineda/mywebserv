/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:48:49 by kpineda-          #+#    #+#             */
/*   Updated: 2026/02/24 20:25:13 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

std::map<int, std::string> HttpResponse::statusMessages;
std::map<std::string, std::string> HttpResponse::mimeTypes;

void HttpResponse::initializeStatusMessages()
{
	if ( !statusMessages.empty() )
		return;
	statusMessages[200] = "OK";
	statusMessages[404] = "Not Found";
	statusMessages[403] = "Forbidden";
	statusMessages[500] = "Internal Server Error";
}

void HttpResponse::initializeMimeTypes()
{
	if ( !mimeTypes.empty() )
		return;
	mimeTypes["html"] = "text/html";
	mimeTypes["htm"] = "text/html";
	mimeTypes["css"] = "text/css";
	mimeTypes["js"] = "application/javascript";
	mimeTypes["jpg"] = "image/jpeg";
	mimeTypes["jpeg"] = "image/jpeg";
	mimeTypes["png"] = "image/png";
	mimeTypes["gif"] = "image/gif";
	mimeTypes["txt"] = "text/plain";
}

void HttpResponse::loadFile(const std::string& path)
{
	std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
	if (file.is_open())
	{
		std::stringstream ss;
		ss << file.rdbuf();
		setBody(ss.str());

		size_t dotPos = path.find_last_of('.');
		if (dotPos != std::string::npos)
		{
			std::string ext = path.substr(dotPos + 1);
			if (mimeTypes.count(ext))
				addHeader("Content-Type", mimeTypes[ext]);
		}
		setStatusCode(200);
	}
	else
	{
		setStatusCode(404);
		setBody("<html><body><h1>404 Not Found</h1></body></html>");
		addHeader("Content-Type", "text/html");
	}
}

HttpResponse::HttpResponse() : version("HTTP/1.1"), statusCode(200), body("")
{
	initializeStatusMessages();
	initializeMimeTypes();
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

void HttpResponse::clear()
{
	version = "HTTP/1.1";
	statusCode = 200;
	body.clear();
	headers.clear();
}