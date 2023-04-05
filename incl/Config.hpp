#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Config.hpp"

class Config
{
	public:
		Config(void);
		~Config(void);
		Config(const Config&);

		Config& operator=(const Config&);

		void loadConfigFile(std::string);
		void parseConfigFile();
	
	private:		
		std::vector<Server>	_servers;

};

#endif

/*
chat config for apache:
# Set the server name
ServerName example.com

# Set the document root directory
DocumentRoot /var/www/html

# Set the default file to serve
DirectoryIndex index.html

# Configure access control
<Directory /var/www/html>
    AllowOverride None
    Require all granted
</Directory>

# Enable server-side scripting
AddHandler cgi-script .cgi

# Enable SSL/TLS encryption
SSLEngine on
SSLProtocol all -SSLv3 -TLSv1 -TLSv1.1
SSLHonorCipherOrder on
SSLCipherSuite ECDHE-ECDSA-AES128-GCM-SHA256:ECDHE-RSA-AES128-GCM-SHA256:ECDHE-ECDSA-AES256-GCM-SHA384:ECDHE-RSA-AES256-GCM-SHA384:DHE-RSA-AES128-GCM-SHA256:DHE-RSA-AES256-GCM-SHA384:ECDHE-ECDSA-AES128-SHA256:ECDHE-RSA-AES128-SHA256:ECDHE-ECDSA-AES256-SHA384:ECDHE-RSA-AES256-SHA384:DHE-RSA-AES128-SHA256:DHE-RSA-AES256-SHA256

# Redirect non-HTTPS traffic to HTTPS
RewriteEngine on
RewriteCond %{HTTPS} off
RewriteRule (.*) https://%{HTTP_HOST}%{REQUEST_URI} [R=301,L]

*/