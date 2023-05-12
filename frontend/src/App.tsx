import './App.css'
import Canvas from './features/canvas'

function App() {
  return (
    <div className='w-screen h-screen overflow-hidden
      dark:border-dark-primary dark:bg-dark-primary dark:text-dark-text
    '>
      <Canvas />
    </div>
  )
}

export default App
