import React, { useEffect, useRef } from 'react'

type Props = {}

const Canvas = (props: Props) => {

    const canvasRef = useRef<HTMLCanvasElement>(null)

    useEffect(() => {
        const canvas = canvasRef.current
        const context = canvas?.getContext('2d')
        if (!context) return
    }, [])



    return (
        <canvas id="canvas"
            className='w-full h-full absolute top-0 left-0'
            ref={canvasRef}></canvas>
    )
}

export default Canvas