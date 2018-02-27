let os = require('os')
console.log('Node.js creating server in: ', os.hostname())


require('http')
  .createServer((req, res) => { res.end('Hello World!') }).listen(8080)
