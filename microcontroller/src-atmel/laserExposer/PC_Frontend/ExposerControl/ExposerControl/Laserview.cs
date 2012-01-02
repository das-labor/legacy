using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Drawing.Drawing2D;


namespace ExposerControl
{
    public partial class Laserview : UserControl
    {
        public Laserview()
        {
            InitializeComponent();
        }

        Brush selectedBrush = new SolidBrush(Color.Blue);
        //Matrix lastTrans = new Matrix();
        Rectangle lastRectangle = new Rectangle(2,2,10,10);

        Point mirrorPos = new Point(400, 50);
        Pen mirrorPen = new Pen(Color.Black);
        Pen laserPen  = new Pen(Color.Red);

        PointF[] mirrorPoints = new PointF[6];
        PointF[] laserPoints = new PointF[] { new PointF(0, 200), new PointF(800, -50) };
        PointF[] laserFinalPoints = new PointF[3];
        


        double mirrorAngle = 0;
        double mirrorDiameter = 50;

        protected override void OnPaint(PaintEventArgs e)
        {
            //e.Graphics.Transform= m_trans;
            Graphics g = e.Graphics; 

            //draw mirror
                
            for (int x = 0; x < 6;x++ )
            {
                g.DrawLine(mirrorPen, mirrorPoints[x] , mirrorPoints[(x+1)%6] );
            
            }


            g.DrawLine(laserPen, laserFinalPoints[0], laserFinalPoints[1]);
            g.DrawLine(laserPen, laserFinalPoints[1], laserFinalPoints[2]);


            //g.FillRectangle(selectedBrush, lastRectangle);
       

        }


        public static PointF FindLineIntersection(PointF start1, PointF end1, PointF start2, PointF end2)
        {
            float denom = ((end1.X - start1.X) * (end2.Y - start2.Y)) - ((end1.Y - start1.Y) * (end2.X - start2.X));

            //  AB & CD are parallel 
            if (denom == 0)
                return PointF.Empty;

            float numer = ((start1.Y - start2.Y) * (end2.X - start2.X)) - ((start1.X - start2.X) * (end2.Y - start2.Y));

            float r = numer / denom;

            float numer2 = ((start1.Y - start2.Y) * (end1.X - start1.X)) - ((start1.X - start2.X) * (end1.Y - start1.Y));

            float s = numer2 / denom;

            if ((r < 0 || r > 1) || (s < 0 || s > 1))
                return PointF.Empty;

            // Find intersection point
            PointF result = new PointF();
            result.X = start1.X + (r * (end1.X - start1.X));
            result.Y = start1.Y + (r * (end1.Y - start1.Y));

            return result;
        }


        double abs(PointF a1, PointF a2) {
            return Math.Sqrt( (a2.X - a1.X) * (a2.X - a1.X) + (a2.Y - a1.Y) * (a2.Y - a1.Y) );
        }

        double scalarProd(PointF a1, PointF a2, PointF b1, PointF b2) {
            return (a2.X - a1.X) * (b2.X - b1.X) + (a2.Y - a1.Y) * (b2.Y - b1.Y);
        }

        double angle(PointF a1, PointF a2, PointF b1, PointF b2) {
            return Math.Acos( scalarProd(a1, a2, b1, b2) / (abs(a1, a2) * abs(b1, b2)));
        }

        public void calculate()
        {

            for (int x = 0; x < 6; x++)
            {
                double a = mirrorAngle + x * (Math.PI / 3);
                mirrorPoints[x] = new PointF((float)(mirrorPos.X + Math.Cos(a) * mirrorDiameter), (float)(mirrorPos.Y + Math.Sin(a) * mirrorDiameter));
            }

            PointF intersection = PointF.Empty;
            PointF intersect;


            laserFinalPoints[0] = laserPoints[0];
            laserFinalPoints[1] = laserPoints[1];

            double ang = 0;

            for (int x = 0; x < 6; x++) {
                intersect = FindLineIntersection(mirrorPoints[x], mirrorPoints[(x + 1) % 6], laserPoints[0], laserPoints[1]);
                if (intersect != PointF.Empty) {
                    if (intersection == PointF.Empty)
                    {
                        intersection = intersect;
                        ang = angle(laserPoints[0], laserPoints[1], mirrorPoints[x], mirrorPoints[(x + 1) % 6]);
                    }
                    else 
                    { 
                        if(abs(laserPoints[0], intersect) < abs(laserPoints[0], intersection )){
                            intersection = intersect;
                            ang = angle(laserPoints[0], laserPoints[1], mirrorPoints[x], mirrorPoints[(x + 1) % 6]);
                        }
                    }
                }
            }

            laserFinalPoints[1] = intersection;

            ang *= -2;

            Matrix m = new Matrix();
            m.Reset();
            m.RotateAt((float)(ang * 180 / Math.PI), intersection);

            PointF[] tmp = new PointF[] { laserPoints[1] };

            m.TransformPoints(tmp);

            laserFinalPoints[2] = tmp[0];


        }
    
        public void SetMirrorAngle(double a){
            mirrorAngle = a / 180 * Math.PI;
            calculate();
            Invalidate();
        }
    
    }
}
