/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kpineda- <kpineda-@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 13:48:49 by kpineda-          #+#    #+#             */
/*   Updated: 2026/03/02 16:57:31 by kpineda-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

// Static member definitions
std::map<int, std::string> HttpResponse::statusMessages;
std::map<std::string, std::string> HttpResponse::mimeTypes;

HttpResponse::HttpResponse() : version("HTTP/1.1"), statusCode(200), body("")
{
	initializeStatusMessages();
	initializeMimeTypes();
}

HttpResponse::~HttpResponse() 
{
}

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

void HttpResponse::setStatusCode(int code)
{
	statusCode = code;
}

void HttpResponse::addHeader(const std::string& key, const std::string& value)
{
	headers[key] = value;
}

void HttpResponse::setBody(const std::string& body)
{
	this->body = body;

	std::stringstream ss;
	ss << body.length();
	addHeader("Content-Length", ss.str());
}

void HttpResponse::clear()
{
	version = "HTTP/1.1";
	statusCode = 200;
	body.clear();
	headers.clear();
}

std::string HttpResponse::toLower(std::string s)
{
	for (size_t i = 0; i < s.length(); ++i)
		s[i] = std::tolower(static_cast<unsigned char>(s[i]));
	return s;
}

void HttpResponse::loadFile(const std::string& path)
{
	struct stat path_stat;
	// Verify if the file exists and is a regular file
	if (stat(path.c_str(), &path_stat) != 0)
	{
		// File doesn't exist or can't be accessed
		setStatusCode(404);
		setBody("<html><body><h1>404 Not Found</h1><p>The requested resource was not found on this server.</p></body></html>");
		addHeader("Content-Type", "text/html");
		return;
	}
	// Is it a directory?
	if (S_ISDIR(path_stat.st_mode))
	{
		setStatusCode(403);
		setBody("<html><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>");
		addHeader("Content-Type", "text/html");
		std::string index_path = path + "/index.html";
		struct stat index_stat;
		if (stat(index_path.c_str(), &index_stat) == 0 )
			loadFile(index_path); // Try to load index.html if it exists
		return;
	}
	// It's a regular file, try to read it
	if (S_ISREG(path_stat.st_mode))
	{
		std::ifstream file(path.c_str(), std::ios::in | std::ios::binary);
		if (file.is_open())
		{
			// 1. Read file content into body
			std::stringstream ss;
			ss << file.rdbuf();
			setBody(ss.str());
			
			// 2. Find file extension
			size_t dot_pos = path.find_last_of(".");
			if (dot_pos != std::string::npos)
			{
				// Extract extension
				std::string ext = path.substr(dot_pos + 1);

				// Convert to lowercase for case-insensitive matching
				std::string lowerExt = toLower(ext);
				
				// Search for MIME type based on extension
				if (mimeTypes.count(lowerExt))
					addHeader("Content-Type", mimeTypes[lowerExt]);
				else
					addHeader("Content-Type", "application/octet-stream");
			}
			setStatusCode(200);
			file.close();
		}
		else
		{
			// File exists but can't be opened (permissions issue)
			setStatusCode (403);
			setBody("<html><body><h1>403 Forbidden</h1><p>You don't have permission to access this resource.</p></body></html>");
			addHeader("Content-Type", "text/html");
		}
	}
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