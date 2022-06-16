CREATE TABLE prog
(
    id int NOT NULL IDENTITY(1, 1), 
    name nvarchar(255) NOT NULL, 
    released_at date NOT NULL, 
    platform nvarchar(255) NOT NULL, 
    appointment nvarchar(255) NOT NULL, 
    CONSTRAINT [PK_prog_id] PRIMARY KEY (id)
)


CREATE TABLE users
(
    id int NOT NULL IDENTITY(1, 1), 
    id_prog int NOT NULL, 
    full_name nvarchar(255) NOT NULL, 
    country nvarchar(255) NOT NULL, 
    CONSTRAINT [PK_users_id] PRIMARY KEY (id), 
    CONSTRAINT [users$users_ibfk_1] FOREIGN KEY (id_prog) REFERENCES prog (id)
)


CREATE TABLE dirs
(
    id_dir int NOT NULL IDENTITY(1, 1), 
    id_prog int NOT NULL, 
    name nvarchar(255) NOT NULL, 
    CONSTRAINT [PK_dirs_id_dir] PRIMARY KEY (id_dir), 
    CONSTRAINT dirs$dirs_ibfk_1 FOREIGN KEY (id_prog) REFERENCES prog (id) ON DELETE CASCADE
)

CREATE TABLE files
(
    id_files int NOT NULL IDENTITY(1, 1), 
    id_dir int NOT NULL, 
    name nvarchar(255) NOT NULL, 
    extension nvarchar(15) NOT NULL, 
    size int NOT NULL, 
    created_at date NOT NULL,
    modified_at datetime NULL
    CONSTRAINT [PK_files_id_files] PRIMARY KEY (id_files), 
    CONSTRAINT files$files_ibfk_1 FOREIGN KEY (id_dir) REFERENCES dirs (id_dir) ON DELETE CASCADE
)

SELECT name AS 'Название', platform AS 'Платформа', appointment AS 'Назначение', CASE WHEN released_at > DATEADD(year, -1, GETDATE()) 
THEN 'Менее года назад' 
WHEN released_at < DATEADD(year, -10, GETDATE()) THEN 'Более 10 лет назад' 
WHEN released_at < DATEADD(year, -5, GETDATE()) THEN 'Более 5 лет назад' 
WHEN released_at < DATEADD(year, -1, GETDATE()) THEN 'Более года назад' 
ELSE 'Очень давно' end as 'Дата выхода', ISNULL((SELECT full_name FROM users WHERE id_prog=prog.id), 'Владелец не указан') as 'Владелец' 
FROM prog

CREATE VIEW usersProgs AS
SELECT p.name as program, p.platform as platform,  u.full_name as owner FROM users u
INNER JOIN prog p ON p.id=u.id_prog

UPDATE usersProgs SET program='Discord' WHERE owner = 'Vladimir'


SELECT f.name AS 'Название файла', (select p.name) AS 'Название программы', (SELECT d.name FROM dirs d WHERE d.id_prog=p.id) 
AS 'Название директории'
FROM (SELECT * FROM prog) p, files f
WHERE f.id_dir in (SELECT id_dir FROM dirs WHERE id_prog=p.id)


SELECT * FROM prog WHERE released_at = (SELECT MAX(released_at) FROM prog)

SELECT * FROM prog p1 WHERE p1.id IN (SELECT id FROM prog p2 WHERE platform='windows' and p2.id=p1.id) 

SELECT name, size FROM files f1 WHERE f1.size IN (SELECT MAX(f2.size) FROM files f2 WHERE f2.id_dir=f1.id_dir)

SELECT * FROM files a WHERE a.size > (SELECT AVG(b.size) FROM files b )

SELECT p.platform, COUNT(*) AS 'Кол-во', ISNULL(fi.extension, 'Другое') as 'Расширение' FROM prog p
LEFT JOIN dirs d on d.id_prog=p.id
LEFT JOIN (SELECT * FROM files f WHERE f.extension='.exe') fi on fi.id_dir=d.id_dir
GROUP BY platform, fi.extension HAVING COUNT(*) > 1


SELECT d.name AS 'Название директории' FROM dirs d
WHERE d.id_prog= ANY(SELECT id FROM prog)

CREATE INDEX users_name_country_idx ON users(full_name, country)
CREATE INDEX prog_name_date_platform_idx ON prog(name, released_at, platform)
CREATE INDEX files_ext_size_date_idx ON files(extension, size, created_at)


CREATE TRIGGER create_dir_trig ON prog
AFTER INSERT AS
BEGIN
DECLARE @id int
DECLARE @name nvarchar(255)
SELECT @id = (SELECT id FROM inserted)
SELECT @name = 'C:\\ProgramFiles\' + (SELECT name FROM inserted)
INSERT INTO dirs (id_prog, name) VALUES (@id, @name)
END

CREATE TRIGGER create_file_trig ON files
AFTER INSERT AS
BEGIN
DECLARE @id_file int
SELECT @id_file = (SELECT id_files FROM inserted)
UPDATE files SET modified_at = GETDATE() WHERE id_files = @id_file
END

CREATE TRIGGER update_file_trig ON files
AFTER UPDATE AS
BEGIN
DECLARE @id_file int
SELECT @id_file = (SELECT id_files FROM inserted)
UPDATE files SET modified_at = GETDATE() WHERE id_files = @id_file
END


CREATE PROCEDURE add_user(
	@IdProg int = null,
	@Name nvarchar(max),
	@Country nvarchar(max)
)
AS
BEGIN
INSERT INTO users (id_prog, full_name, country) VALUES (@IdProg, @Name, @Country)
END

CREATE PROCEDURE add_prog(
	@Name nvarchar(max),
	@Release DATE,
	@Platform nvarchar(max),
	@Appointment nvarchar(max)
)
AS
BEGIN
INSERT INTO prog (name, released_at, platform,appointment) VALUES (@Name, @Release, @Platform, @Appointment)
END


CREATE PROCEDURE add_dir(
	@IdProg INT = NULL,
	@Name nvarchar(max)
)
AS
BEGIN
INSERT INTO dirs (id_prog, name) VALUES (@IdProg, @Name)
END

CREATE PROCEDURE add_file(
	@IdDir INT = NULL,
	@Name nvarchar(max),
	@Extension nvarchar(max),
	@Size int,
	@Created DATE
)
AS
BEGIN
INSERT INTO files (id_dir, name, extension, size, created_at) VALUES (@IdDir, @Name, @Extension, @Size, @Created)
END

CREATE PROCEDURE update_user(
	@ID int,
	@IdProg int = null,
	@Name nvarchar(max),
	@Country nvarchar(max)
)
AS
BEGIN
UPDATE users SET id_prog=@IdProg, full_name=@Name,country=@Country WHERE id=@ID
END

CREATE PROCEDURE update_prog(
	@ID int,
	@Name nvarchar(max),
	@Release DATE,
	@Platform nvarchar(max),
	@Appointment nvarchar(max)
)
AS
BEGIN
UPDATE prog SET name=@Name, released_at=@Release, platform=@Platform, appointment=@Appointment WHERE id=@ID
END

CREATE PROCEDURE update_dir(
	@ID int,
	@IdProg INT = NULL,
	@Name nvarchar(max)
)
AS
BEGIN
UPDATE dirs SET id_prog=@IdProg, name=@Name WHERE id_dir=@ID
END

CREATE PROCEDURE update_file(
	@ID int,
	@IdDir INT = NULL,
	@Name nvarchar(max),
	@Extension nvarchar(max),
	@Size int,
	@Created DATE
)
AS
BEGIN
UPDATE files SET id_dir=@IdDir, name=@Name, extension=@Extension, size=@Size, created_at=@Created WHERE id_files=@ID
END

CREATE PROCEDURE delete_user(
	@ID int
)
AS
BEGIN
DELETE FROM users WHERE id=@ID
END

CREATE PROCEDURE delete_prog(
	@ID int
)
AS
BEGIN
DELETE FROM prog WHERE id=@ID
END

CREATE PROCEDURE delete_dir(
	@ID int
)
AS
BEGIN
DELETE FROM dirs WHERE id_dir=@ID
END

CREATE PROCEDURE delete_file(
	@ID int
)
AS
BEGIN
DELETE FROM files WHERE id_files=@ID
END


BEGIN TRANSACTION
DECLARE @f_count INT

   DELETE FROM files WHERE size < 10
   SELECT @f_count=COUNT(*) FROM files
   IF @f_count < 5
		ROLLBACK
   ELSE
		COMMIT TRANSACTION


DECLARE @curSize int
DECLARE @testCur CURSOR
SET @testCur = CURSOR SCROLL FOR
	SELECT size FROM files

OPEN @testCur
FETCH NEXT FROM @testCur INTO @curSize
WHILE @@FETCH_STATUS = 0  BEGIN
	PRINT @curSize
	UPDATE files
      SET    size = @curSize + 10
      WHERE  CURRENT OF @testCur;
	FETCH NEXT FROM @testCur INTO @curSize
END
CLOSE @testCur
DEALLOCATE @testCur

CREATE FUNCTION countProgsByName(
@uname varchar(255)
)
RETURNS int AS
BEGIN
	
	DECLARE @countID int
	SELECT @countID=COUNT(id) FROM users WHERE full_name=@uname
	RETURN @countID
END

CREATE FUNCTION fileSize(
@size int
)
RETURNS TABLE AS RETURN 
SELECT * FROM files WHERE size<@size


CREATE LOGIN user1 WITH PASSWORD='password2002', DEFAULT_DATABASE=progs
CREATE USER user1 FOR LOGIN user1 
ALTER LOGIN user1 ENABLE
GRANT SELECT ON dbo.prog TO user1
GRANT SELECT ON dbo.users TO user1

