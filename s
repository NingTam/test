CREATE DATABASE student_attendance;
USE student_attendance;

CREATE TABLE students (
  roll INT PRIMARY KEY,
  name VARCHAR(100),
  class VARCHAR(10),
  section VARCHAR(10),
  attendance VARCHAR(10),
  status VARCHAR(10) DEFAULT ''
);

INSERT INTO students (roll, name, class, section, attendance) VALUES
(1, 'Tomba', '12', 'A', '85%'),
(2, 'Ram', '12', 'A', '90%'),
(3, 'Shyam', '12', 'B', '78%');
