-- Create database user and grant privileges
CREATE USER 'flask_user'@'localhost' IDENTIFIED BY 'flask_password123';
GRANT ALL PRIVILEGES ON company.* TO 'flask_user'@'localhost';
FLUSH PRIVILEGES;