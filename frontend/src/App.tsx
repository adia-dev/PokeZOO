import { useState } from 'react'
import './App.css'
import Canvas from './features/canvas'

function App() {
  const [count, setCount] = useState(0)

  return (
    <div className='w-screen h-screen overflow-hidden
      dark:border-dark-primary dark:bg-dark-primary dark:text-dark-text
    '>
      <Canvas />
    </div>
  )
}

export default App
