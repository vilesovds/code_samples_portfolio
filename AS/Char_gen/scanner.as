package {
	import flash.display.MovieClip;
	import flash.events.Event;
	import fl.transitions.Tween;
	import fl.transitions.easing.*;
	import fl.transitions.TweenEvent;
	import flash.filters.*;
	public class scanner extends MovieClip{
		private var line:mc_line;
		public var distance:uint=100;
		private const duration=1;
		private var _position:uint;
		private var glow:GlowFilter;
		public var etw:Boolean=false;
		public var foo:Function;
		public function scanner()
		{
			
			glow=new GlowFilter();
			glow.color = 0x00FF00;
			glow.alpha = 0.9;
			glow.blurX = 5;
			glow.blurY = 1;
			glow.quality = BitmapFilterQuality.HIGH;
			
			
			line=new mc_line();
			line.filters=[glow];
			line.addEventListener(Event.ENTER_FRAME,mcplay);
			this.addChild(line);
			this.addEventListener(Event.ENTER_FRAME,onframe);
		}
		private function mcplay(e:Event):void{
		
		 if(line!=null){
			if (glow.blurY<10){
				glow.blurY++			
				glow.color=0x00FF00;
			}
			else{
				if(glow.blurY==20){
					glow.blurY=0;
				}
				glow.blurY++;
				glow.color=0xFFFFFF;
			}
			line.filters=[glow];
			
			
			if(line.y>=distance-1){
				removemc();
				etw=true;
			}
			else{

				etw=false;
			}

			
		 }
		}

		public function set position(y:uint):void
		{
			_position=y;
			var tw:Tween;
			if(line!=null){
				tw=new Tween(line,"y",Strong.easeOut,line.y,(y/100)*distance,duration,true);
				//tw.addEventListener(TweenEvent.MOTION_FINISH,endtween);
			}
		}

		private function endtween(e:TweenEvent):void
		{
	

			if(line.y==distance){
				removemc();
				etw=true;
			}
			else{

				etw=false;
			}

		}
		function onframe(e:Event):void{
				position=_position;
		}
		private function removemc(){
			// foo();
			 trace('rem');
			if(line!=null){
				//line.parent.removeChild(line);		
				line.removeEventListener(Event.ENTER_FRAME,mcplay);
				line.stop();
				line.visible=false;		
				
			}
		}
	}
	
}