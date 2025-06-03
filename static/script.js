    let selectedFile = null;

    document.getElementById('csvFile').addEventListener('change', function (e) {
        selectedFile = e.target.files[0];
        if (!selectedFile) return;

        const reader = new FileReader();
        reader.onload = function (event) {
            const text = event.target.result;
            displayCSV(text);
            document.getElementById('uploadBtn').disabled = false;
        };
        reader.readAsText(selectedFile);
    });

    document.getElementById('uploadBtn').addEventListener('click', function () {
        const csvText = tableToCSV();
        if (!csvText) {
            alert('No table data to upload!');
            return;
        }

        const blob = new Blob([csvText], { type: 'text/csv' });
        const formData = new FormData();
        formData.append('file', blob, selectedFile ? selectedFile.name : 'edited.csv');

        fetch('/upload', {
            method: 'POST',
            body: formData
        })
            .then(response => {
                if (!response.ok) throw new Error("Upload failed");
                return response.text();
            })
            .then(data => alert('File uploaded successfully!'))
            .catch(err => alert('Upload error: ' + err.message));
    });

    function handleDownload(btn, { artist, song, url }) {
        if (!url) {
            console.error('No URL found!');
            return;
        }

        btn.disabled = true;
        btn.textContent = '⏳';

        fetch('/dl', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ artist, url, song})
        })
            .then(response => {
                if (!response.ok) throw new Error('Request failed');
                return response.text();
            })
            .then(data => {
                btn.textContent = '✅';
                console.log(`Downloaded ${song} by ${artist}:`, data);
            })
            .catch(err => {
                btn.textContent = '❌';
                console.error(`Error downloading ${song}:\n${err.message}`);
            })
            .finally(() => {
                if (btn.textContent === '❌') {
                    setTimeout(() => {
                        btn.disabled = false;
                        btn.textContent = 'Download';
                    }, 3000);
                }
            });
    }

    function displayCSV(csv) {
        const rows = csv.trim().split('\n').map(row => row.split(';').map(cell => cell.trim()));
        if (rows.length === 0) return;

        const headers = rows[0];
        const rowData = rows.slice(1);
        const table = document.createElement('table');

        const thead = document.createElement('tr');
        headers.forEach(header => {
            const th = document.createElement('th');
            th.textContent = header;
            thead.appendChild(th);
        });

        const actionTh = document.createElement('th');
        actionTh.textContent = 'Action';
        thead.appendChild(actionTh);
        table.appendChild(thead);

        rowData.forEach(dataRow => {
            const tr = document.createElement('tr');

            dataRow.forEach(cell => {
                const td = document.createElement('td');
                td.contentEditable = true;

                if (cell.startsWith('http')) {
                    td.innerHTML = `<a href="${cell}" target="_blank">${cell}</a>`;
                } else {
                    td.textContent = cell;
                }

                tr.appendChild(td);
            });

            const btnTd = document.createElement('td');
            const btn = document.createElement('button');
            btn.textContent = 'Download';

            btn.addEventListener('click', () => {
                const cells = tr.querySelectorAll('td');
                const song = cells[0].textContent.trim();
                const artist = cells[1].textContent.trim();
                const urlCell = cells[2];
                const link = urlCell.querySelector('a');
                const url = link ? link.href : urlCell.textContent.trim();

                handleDownload(btn, { artist, song, url });
            });

            btnTd.appendChild(btn);
            tr.appendChild(btnTd);
            table.appendChild(tr);
        });

        const output = document.getElementById('output');
        output.innerHTML = '';

        const downloadAllBtn = document.createElement('button');
        downloadAllBtn.textContent = 'Download All';
        downloadAllBtn.style.margin = '10px 0';

        downloadAllBtn.addEventListener('click', () => {
            const rows = table.querySelectorAll('tr');
            for (let i = 1; i < rows.length; i++) {
                const cells = rows[i].querySelectorAll('td');
                const btn = cells[cells.length - 1].querySelector('button');
                const song = cells[0].textContent.trim();
                const artist = cells[1].textContent.trim();
                const urlCell = cells[2];
                const link = urlCell.querySelector('a');
                const url = link ? link.href : urlCell.textContent.trim();

                if (!url) continue;
                handleDownload(btn, { artist, song, url });
            }
        });

        output.appendChild(downloadAllBtn);
        output.appendChild(table);
    }

    function tableToCSV() {
        const table = document.querySelector('#output table');
        if (!table) return '';

        let csv = '';
        for (const row of table.rows) {
            const cells = Array.from(row.cells).slice(0, -1).map(td => {
                const link = td.querySelector('a');
                return link ? link.href : td.textContent.trim();
            });
            csv += cells.join(';') + '\n';
        }
        return csv.trim();
    }