const express = require('express');
const fs = require('fs');
const app = express();

app.use(express.json());
app.use(express.static(__dirname));

function clasificarLuz(valor) {
  if (valor === null || valor === undefined) return 'Sin dato';
  if (valor < 500)  return 'No hay luz';
  if (valor < 1500) return 'Poca luz';
  if (valor < 2800) return 'Luz regular';
  return 'Mucha luz';
}

app.post('/datos', (req, res) => {
  const dato = {
    digital: req.body.digital,
    analogico: req.body.analogico,
    hora: new Date().toLocaleString('es-PE', { timeZone: 'America/Lima' }),
    timestamp: new Date().toISOString(),
    estado: clasificarLuz(req.body.analogico)
  };

  let historial = [];
  if (fs.existsSync('historial.json')) {
    historial = JSON.parse(fs.readFileSync('historial.json'));
  }

  historial.unshift(dato);

  // Guardar hasta 5000 registros
  if (historial.length > 5000) historial = historial.slice(0, 5000);

  fs.writeFileSync('historial.json', JSON.stringify(historial));
  console.log('Dato recibido:', dato);
  res.json({ ok: true });
});

// Obtener historial con filtros opcionales
app.get('/datos', (req, res) => {
  if (!fs.existsSync('historial.json')) return res.json([]);

  let historial = JSON.parse(fs.readFileSync('historial.json'));

  const { desde, hasta } = req.query;

  if (desde || hasta) {
    historial = historial.filter(d => {
      const t = new Date(d.timestamp).getTime();
      const desdeT = desde ? new Date(desde).getTime() : 0;
      const hastaT = hasta ? new Date(hasta).getTime() : Infinity;
      return t >= desdeT && t <= hastaT;
    });
  }

  res.json(historial);
});
// Estado del sensor (activo o pausado)
let estadoSensor = 'activo';

app.get('/estado', (req, res) => {
  res.json({ estado: estadoSensor });
});

app.post('/estado', (req, res) => {
  estadoSensor = req.body.estado;
  console.log('Sensor:', estadoSensor);
  res.json({ ok: true, estado: estadoSensor });
});
// Comando de sueño
let comandoSueno = 'despierto';

app.get('/comando', (req, res) => {
  res.json({ comando: comandoSueno });
});

app.post('/comando', (req, res) => {
  comandoSueno = req.body.comando;
  console.log('Comando:', comandoSueno);
  res.json({ ok: true, comando: comandoSueno });
});
app.listen(4000, () => {
  console.log('Servidor sensor corriendo en puerto 4000');
});
