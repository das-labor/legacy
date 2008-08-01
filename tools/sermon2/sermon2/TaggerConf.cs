// TaggerConf.cs
// 
// Copyright (C) 2008 Daniel Otte
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

using System;

namespace sermon2
{
    
    
    public partial class TaggerConf : Gtk.Bin
    {
        private TaggerConfig config;
        private TaggerDataConf[] tgdc;
        public TaggerConf(TaggerConfig config)
        {
            this.Build();
            this.config = config;
            tgdc = new TaggerDataConf[0];
            backgroundCButton.Color = new Gdk.Color(0xff, 0xff, 0xff);     
            tgdc = new TaggerDataConf[0];
            frame2.Sensitive = dataCheckButton.Active;
            SetGuiFromConfig();
            config.dataChanged += SetGuiFromConfig;
        }
        
        public void SetGuiFromConfig(){
            nameLabel.Markup = "<b>" + config.id +  
                               " / " + config.name + "</b>";
            foregroundCheckButton.Active = config.defTag.ForegroundSet;
            foregroundCButton.Color = config.defTag.ForegroundGdk;
            backgroundCheckButton.Active = config.defTag.BackgroundSet;
            backgroundCButton.Color = config.defTag.BackgroundGdk;
            foregroundCButton.Sensitive = foregroundCheckButton.Active;
            backgroundCButton.Sensitive = backgroundCheckButton.Active;
            dataCheckButton.Active = config.dataTag;
            int j=0;
            for(int i=0;i<256;++i){
                if(config.dataTags[i]!=null){
                   if(tgdc.Length<=j){
                       Array.Resize(ref tgdc, j+1);
                       tgdc[j] = new TaggerDataConf((byte)i, config);
                       vbox2.Add(tgdc[j]);
                       vbox2.ShowAll();
                   }
                   tgdc[j].SetGuiFromConfig();
                   ++j;
                }
            }
        }

        public void SetGuiFromConfig(object changer, object changed){
            System.Console.WriteLine("taggerConf: SetGuiFromConfig");
            if(changer!=this){
                System.Console.WriteLine(" -> do");
                SetGuiFromConfig();
            }
        }
        
        protected virtual void OnAddTaggerDataConfbuttonReleased (object sender, System.EventArgs e)
        {   
            if(valueSpinButton.Value == -1)
                return;
            Array.Resize(ref tgdc, tgdc.Length+1);
            tgdc[tgdc.Length-1] = new TaggerDataConf((byte)valueSpinButton.Value, config);
            vbox1.Add(tgdc[tgdc.Length-1]);
            vbox1.ShowAll();
            
            System.Console.WriteLine("d");
        }


        protected virtual void OnForegroundCheckButtonReleased (object sender, System.EventArgs e)
        {
            foregroundCButton.Sensitive = foregroundCheckButton.Active;
            config.defTag.ForegroundSet = foregroundCheckButton.Active;
            config.OnGUI_Changed(this);
        }

        protected virtual void OnBackgroundCheckButtonReleased (object sender, System.EventArgs e)
        {
            backgroundCButton.Sensitive = backgroundCheckButton.Active;
            config.defTag.BackgroundSet = backgroundCheckButton.Active;
            config.OnGUI_Changed(this);
        }

        protected virtual void OnDataCheckButtonReleased (object sender, System.EventArgs e)
        {
            frame2.Sensitive = dataCheckButton.Active;
            config.dataTag   = dataCheckButton.Active;
            config.OnGUI_Changed(this);
        }

        protected virtual void OnForegroundCButtonColorSet (object sender, System.EventArgs e)
        {
            config.defTag.ForegroundGdk = foregroundCButton.Color;
            config.OnGUI_Changed(this);
        }

        protected virtual void OnBackgroundCButtonColorSet (object sender, System.EventArgs e)
        {
            config.defTag.BackgroundGdk = backgroundCButton.Color;
            config.OnGUI_Changed(this);
        }
        
    }
}
