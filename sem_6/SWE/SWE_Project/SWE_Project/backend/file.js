import express from 'express'

const app = express()

app.use(express.json())

app.post('/users', async (req, res) => {
  const { password, username } = req.body
  if (!password || !username) {
    res.sendStatus(400)
    return
  }

  res.send({ userId: 0 })
})

app.put('/users/:id', async (req, res) => {
  // Here you would update the user with req.params.id
  // with the new data in req.body
  res.sendStatus(200)
})

app.delete('/posts/:id', async (req, res) => {
  // Here you would delete the post with req.params.id
  res.sendStatus(200)
})

app.post('/posts', async (req, res) => {
  // Here you would create a new post with the data in req.body
  res.status(201).send({ postId: 0 })
})

export default app