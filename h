<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<title>Student Attendance</title>
<style>
  /* Same CSS styling as before for cards and modal */
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body {
    font-family: Arial, sans-serif;
    padding: 20px;
    background: #f7f7f7;
    display: flex;
    justify-content: center;
  }
  .wrapper {
    width: 100%;
    max-width: 800px;
  }
  h2 {
    text-align: center;
    margin-bottom: 20px;
    color: #333;
  }
  .container {
    display: flex;
    flex-wrap: wrap;
    justify-content: center;
    gap: 15px;
  }
  .student {
    position: relative;
    background: linear-gradient(135deg, #f0f0f0, #e0e0e0);
    padding: 15px;
    width: 180px;
    min-height: 120px;
    text-align: center;
    border-radius: 15px;
    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
    cursor: pointer;
    user-select: none;
    transition: all 0.3s ease;
  }
  .student.active {
    transform: scale(1.05);
    box-shadow: 0 6px 12px rgba(0, 0, 0, 0.15);
    background: linear-gradient(135deg, #e0e0e0, #d0d0d0);
  }
  .student.present {
    background: linear-gradient(135deg, #d4edda, #c3e6cb);
  }
  .student.present.active {
    background: linear-gradient(135deg, #c3e6cb, #b2d8b9);
  }
  .student.absent {
    background: linear-gradient(135deg, #f8d7da, #f5c6cb);
  }
  .student.absent.active {
    background: linear-gradient(135deg, #f5c6cb, #f2b5ba);
  }
  .content {
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
  }
  .top-row {
    display: flex;
    align-items: center;
    justify-content: center;
    gap: 10px;
    margin-bottom: 10px;
  }
  .roll-number {
    font-size: 20px;
    font-weight: bold;
    color: #333;
    user-select: text;
  }
  .info-icon {
    font-size: 20px;
    cursor: pointer;
    color: #007bff;
    user-select: none;
    transition: transform 0.2s;
  }
  .info-icon:active {
    transform: scale(1.2);
    filter: brightness(0.8);
  }
  .hover-icons {
    display: flex;
    gap: 10px;
    opacity: 0;
    transform: translateX(100%);
    transition: opacity 0.3s ease, transform 0.4s ease;
    margin-bottom: 5px;
  }
  .student.active .hover-icons {
    opacity: 1;
    transform: translateX(0);
  }
  .hover-icons span {
    cursor: pointer;
    font-size: 20px;
    padding: 8px;
    border-radius: 50%;
    background: white;
    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.2);
    user-select: none;
    transition: transform 0.2s, filter 0.2s;
  }
  .hover-icons span:active {
    transform: scale(1.2);
    filter: brightness(0.8);
  }

  /* Popup modal overlay */
  #info-modal {
    position: fixed;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: rgba(0,0,0,0.5);
    display: none;
    align-items: center;
    justify-content: center;
    z-index: 9999;
  }
  #info-modal.active {
    display: flex;
  }
  #info-modal .modal-content {
    background: white;
    border-radius: 12px;
    padding: 20px 25px;
    max-width: 320px;
    width: 90%;
    box-shadow: 0 8px 20px rgba(0,0,0,0.25);
    position: relative;
    user-select: text;
  }
  #info-modal .close-btn {
    position: absolute;
    top: 12px;
    right: 15px;
    font-size: 22px;
    font-weight: bold;
    cursor: pointer;
    color: #555;
    user-select: none;
  }
  #info-modal .close-btn:hover {
    color: #000;
  }
  #info-modal p {
    margin-bottom: 10px;
    font-size: 16px;
  }
  #info-modal strong {
    color: #333;
  }
</style>
</head>
<body>
<div class="wrapper">
  <h2>Student Attendance</h2>
  <div class="container" id="student-container"></div>
</div>

<!-- Info Popup Modal -->
<div id="info-modal" role="dialog" aria-modal="true" aria-labelledby="info-title" tabindex="-1">
  <div class="modal-content">
    <span class="close-btn" id="modal-close" aria-label="Close info popup">&times;</span>
    <h3 id="info-title">Student Info</h3>
    <div id="modal-body"></div>
  </div>
</div>

<script>
  const API_BASE = 'http://127.0.0.1:5000'; // Change if backend runs elsewhere

  const container = document.getElementById("student-container");
  const modal = document.getElementById("info-modal");
  const modalBody = document.getElementById("modal-body");
  const modalCloseBtn = document.getElementById("modal-close");

  // Fetch students from backend and render
  async function loadStudents() {
    try {
      const response = await fetch(`${API_BASE}/students`);
      const students = await response.json();
      renderStudents(students);
    } catch (error) {
      console.error("Error loading students:", error);
      container.innerHTML = '<p>Error loading students.</p>';
    }
  }

  // Render student cards
  function renderStudents(students) {
    container.innerHTML = '';
    students.forEach(student => {
      const studentDiv = document.createElement("div");
      studentDiv.className = `student ${student.status}`;
      studentDiv.setAttribute("data-roll", student.roll);
      studentDiv.innerHTML = `
        <div class="content">
          <div class="top-row">
            <div class="roll-number">${student.roll}</div>
            <span class="info-icon" role="button" aria-label="View info for ${student.name}">ℹ️</span>
          </div>
          <div class="hover-icons">
            <span role="button" aria-label="Mark present" title="Mark present">✔️</span>
            <span role="button" aria-label="Mark absent" title="Mark absent">❌</span>
          </div>
        </div>
      `;

      // Info icon click: show modal
      studentDiv.querySelector(".info-icon").addEventListener("click", e => {
        e.stopPropagation();
        openInfoModal(student);
      });

      // Attendance icons click
      const hoverIcons = studentDiv.querySelectorAll(".hover-icons span");
      hoverIcons[0].addEventListener("click", e => {
        e.stopPropagation();
        confirmAttendance(student.roll, 'present');
      });
      hoverIcons[1].addEventListener("click", e => {
        e.stopPropagation();
        confirmAttendance(student.roll, 'absent');
      });

      // Toggle active card
      studentDiv.addEventListener("click", function(e) {
        if (e.target.closest(".hover-icons span") || e.target.closest(".info-icon")) return;

        const isActive = this.classList.contains("active");
        document.querySelectorAll(".student").forEach(el => el.classList.remove("active"));

        if (!isActive) {
          this.classList.add("active");
        }
      });

      container.appendChild(studentDiv);
    });
  }

  // Open modal popup
  function openInfoModal(student) {
    modalBody.innerHTML = `
      <p><strong>Name:</strong> ${student.name}</p>
      <p><strong>Class:</strong> ${student.class}</p>
      <p><strong>Section:</strong> ${student.section}</p>
      <p><strong>Attendance:</strong> ${student.attendance}</p>
    `;
    modal.classList.add("active");
    modal.focus();
  }

  // Close modal
  modalCloseBtn.addEventListener("click", () => {
    modal.classList.remove("active");
  });

  // Close modal on outside click
  modal.addEventListener("click", e => {
    if (e.target === modal) {
      modal.classList.remove("active");
    }
  });

  // Confirm attendance update, then call backend API
  function confirmAttendance(roll, status) {
    const studentDiv = document.querySelector(`[data-roll='${roll}']`);
    if (!studentDiv) return;
    const currentStatus = studentDiv.classList.contains(status) ? status : '';

    const action = currentStatus === status ? 'remove' : 'mark';
    const confirmText = currentStatus === status ?
      `Remove ${status} mark for Roll No. ${roll}?` :
      `Mark Roll No. ${roll} as ${status}?`;

    if (confirm(confirmText)) {
      updateAttendance(roll, currentStatus === status ? '' : status);
    }
  }

  // Call backend to update attendance
  async function updateAttendance(roll, status) {
    try {
      const response = await fetch(`${API_BASE}/attendance`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ roll, status }),
      });
      const result = await response.json();
      if (response.ok) {
        // Reload students to update UI
        loadStudents();
      } else {
        alert('Error: ' + (result.error || 'Failed to update attendance'));
      }
    } catch (error) {
      alert('Network error while updating attendance');
      console.error(error);
    }
  }

  // Close active card if click outside any student
  document.addEventListener("click", e => {
    if (!e.target.closest(".student")) {
      document.querySelectorAll(".student").forEach(el => el.classList.remove("active"));
    }
  });

  // Initial load
  loadStudents();
</script>
</body>
</html>
