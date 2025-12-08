# Install mysql
```sh
sudo apt update
sudo apt install mysql-server
```

# Connect to mysql from command line, at the first time
## Create user from sql command line
```sh
sudo mysql 
```
```sql
-- Create database user and grant privileges
CREATE USER 'thai'@'localhost' IDENTIFIED BY '!code123';
GRANT ALL PRIVILEGES ON company.* TO 'thai'@'localhost';
FLUSH PRIVILEGES;
```

## Create user from sql file
```sql
sudo mysql < sql/create_user.sql
```

# Show database
```sql
show databases;
```