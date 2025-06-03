from flask import Flask, jsonify, request
from flask_mysqldb import MySQL
from flask_cors import CORS

app = Flask(__name__)
CORS(app)  # Enable CORS for frontend requests

# MySQL connection config
app.config['MYSQL_HOST'] = 'localhost'
app.config['MYSQL_USER'] = 'your_mysql_username'
app.config['MYSQL_PASSWORD'] = 'your_mysql_password'
app.config['MYSQL_DB'] = 'student_attendance'

mysql = MySQL(app)

@app.route('/students', methods=['GET'])
def get_students():
    cur = mysql.connection.cursor()
    cur.execute("SELECT roll, name, class, section, attendance, status FROM students ORDER BY roll")
    rows = cur.fetchall()
    cur.close()
    students = []
    for row in rows:
        students.append({
            'roll': row[0],
            'name': row[1],
            'class': row[2],
            'section': row[3],
            'attendance': row[4],
            'status': row[5]
        })
    return jsonify(students)

@app.route('/attendance', methods=['POST'])
def update_attendance():
    data = request.get_json()
    roll = data.get('roll')
    status = data.get('status')

    if status not in ('present', 'absent', ''):
        return jsonify({'error': 'Invalid status'}), 400

    cur = mysql.connection.cursor()
    # Toggle status logic:
    if status == '':
        cur.execute("UPDATE students SET status = '' WHERE roll = %s", (roll,))
    else:
        cur.execute("UPDATE students SET status = %s WHERE roll = %s", (status, roll))
    mysql.connection.commit()
    cur.close()
    return jsonify({'message': 'Attendance updated'})

if __name__ == '__main__':
    app.run(debug=True)
